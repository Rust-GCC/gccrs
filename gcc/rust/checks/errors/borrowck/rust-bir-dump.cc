#include <numeric>
#include "rust-bir-dump.h"
#include "rust-diagnostics.h"

namespace Rust {
namespace BIR {

constexpr auto indentation = "    ";

uint32_t
get_lifetime_name (Lifetime lifetime_id)
{
  rust_assert (lifetime_id.id >= FIRST_NORMAL_LIFETIME_ID);
  // Start from 1 as rustc does.
  return lifetime_id.id - FIRST_NORMAL_LIFETIME_ID + 1;
}

std::string
get_tyty_name (TyTy::BaseType *tyty)
{
  if (tyty)
    return tyty->get_name ();
  return "unknown";
}

template <typename T, typename FN>
void
print_comma_separated (std::ostream &stream, const std::vector<T> &collection,
		       FN printer)
{
  if (collection.empty ())
    return;
  printer (collection[0]);
  for (auto it = collection.begin () + 1; it != collection.end (); ++it)
    {
      stream << ", ";
      printer (*it);
    }
}

void
renumber_places (const Function &func, std::vector<PlaceId> &place_map)
{
  // Renumbering places to avoid gaps in the place id space.
  // This is needed to match MIR shape.
  size_t next_out_id = 0;

  for (size_t in_id = FIRST_VARIABLE_PLACE; in_id < func.place_db.size ();
       ++in_id)
    {
      const Place &place = func.place_db[in_id];
      if (place.kind == Place::VARIABLE || place.kind == Place::TEMPORARY)
	{
	  place_map[in_id] = next_out_id++;
	}
      else
	{
	  place_map[in_id] = INVALID_PLACE;
	}
    }
}

void
simplify_cfg (Function &func, std::vector<BasicBlockId> &bb_fold_map)
{
  // The BIR builder can generate many useless basic blocks, which contain only
  // a goto.
  // For actual borrow-checking, the folding has little value.

  bool stabilized = false;
  while (!stabilized)
    {
      stabilized = true;
      // BB0 cannot be folded as it is an entry block.
      for (size_t i = 1; i < func.basic_blocks.size (); ++i)
	{
	  const BasicBlock &bb = func.basic_blocks[bb_fold_map[i]];
	  if (bb.statements.empty () && bb.is_goto_terminated ())
	    {
	      auto dst = bb.successors.at (0);
	      if (bb_fold_map[dst] != dst)
		{
		  rust_error_at (
		    UNKNOWN_LOCATION,
		    "BIR DUMP: Cannot fold CFG, because it contains an "
		    "infinite loop with no executable statements.");
		  rust_inform (UNKNOWN_LOCATION,
			       "Continuing with an unfolded CFG.");
		  // Reverting the fold map to the original state.
		  std::iota (bb_fold_map.begin (), bb_fold_map.end (), 0);
		  stabilized = true;
		  break;
		}
	      bb_fold_map[i] = dst;
	      stabilized = false;
	    }
	}
    }
}

void
Dump::go (bool enable_simplify_cfg)
{
  // To avoid mutation of the BIR, we use indirection through bb_fold_map.
  std::iota (bb_fold_map.begin (), bb_fold_map.end (), 0);

  std::iota (place_map.begin (), place_map.end (), 0);

  if (enable_simplify_cfg)
    simplify_cfg (func, bb_fold_map);

  renumber_places (func, place_map);

  stream << "fn " << name << "(";
  print_comma_separated (stream, func.arguments, [this] (PlaceId place_id) {
    stream << "_" << place_map[place_id] << ": "
	   << get_tyty_name (func.place_db[place_id].tyty);
  });
  stream << ") -> " << get_tyty_name (func.place_db[RETURN_VALUE_PLACE].tyty)
	 << " {\n";

  // Print locals declaration.
  for (PlaceId id = FIRST_VARIABLE_PLACE; id < func.place_db.size (); ++id)
    {
      const Place &place = func.place_db[id];
      if (place.kind == Place::VARIABLE || place.kind == Place::TEMPORARY)
	{
	  if (std::find (func.arguments.begin (), func.arguments.end (), id)
	      != func.arguments.end ())
	    continue;
	  stream << indentation << "let _";
	  stream << place_map[id] << ": "
		 << get_tyty_name (func.place_db[id].tyty) << ";\n";
	}
    }

  // Print BBs.
  for (node_bb = 0; node_bb < func.basic_blocks.size (); ++node_bb)
    {
      if (bb_fold_map[node_bb] != node_bb)
	continue; // This BB was folded.

      if (func.basic_blocks[node_bb].statements.empty ()
	  && func.basic_blocks[node_bb].successors.empty ())
	continue;

      bb_terminated = false;

      BasicBlock &bb = func.basic_blocks[node_bb];
      stream << "\n";
      stream << indentation << "bb" << bb_fold_map[node_bb] << ": {\n";
      for (auto &stmt : bb.statements)
	{
	  stream << indentation << indentation;
	  visit (stmt);
	  stream << ";\n";
	}
      if (!bb_terminated)
	{
	  stream << indentation << indentation << "goto -> bb"
		 << bb_fold_map[bb.successors.at (0)] << ";\n";
	}
      stream << indentation << "}\n";
    }

  stream << "}\n";
}
void
Dump::visit (Node &node)
{
  node_place = node.get_place ();
  switch (node.get_kind ())
    {
      case Node::Kind::ASSIGNMENT: {
	visit_place (node.get_place ());
	stream << " = ";
	node.get_expr ().accept_vis (*this);
	break;
      }
    case Node::Kind::SWITCH:
      stream << "switchInt(";
      visit_move_place (node.get_place ());
      stream << ") -> [";
      print_comma_separated (stream, func.basic_blocks[node_bb].successors,
			     [this] (BasicBlockId succ) {
			       stream << "bb" << bb_fold_map[succ];
			     });
      stream << "]";
      bb_terminated = true;
      break;
    case Node::Kind::RETURN:
      stream << "return";
      bb_terminated = true;
      break;
    case Node::Kind::GOTO:
      stream << "goto -> bb"
	     << bb_fold_map[func.basic_blocks[node_bb].successors.at (0)];
      bb_terminated = true;
      break;
    case Node::Kind::STORAGE_DEAD:
      stream << "StorageDead(";
      visit_move_place (node.get_place ());
      stream << ")";
      break;
    case Node::Kind::STORAGE_LIVE:
      stream << "StorageLive(";
      visit_move_place (node.get_place ());
      stream << ")";
      break;
    }
  node_place = INVALID_PLACE;
}

void
Dump::visit_place (PlaceId place_id)
{
  const Place &place = func.place_db[place_id];
  switch (place.kind)
    {
    case Place::TEMPORARY:
    case Place::VARIABLE:
      stream << "_" << place_map[place_id];
      break;
    case Place::DEREF:
      stream << "(";
      stream << "*";
      visit_place (place.path.parent);
      stream << ")";
      break;
    case Place::FIELD:
      stream << "(";
      visit_place (place.path.parent);
      stream << ".";
      stream << place.variable_or_field_index;
      stream << ": " << get_tyty_name (place.tyty) << ")";
      break;
    case Place::INDEX:
      stream << "(";
      visit_place (place.path.parent);
      stream << "[]";
      stream << ": " << get_tyty_name (place.tyty) << ")";
      break;
    case Place::CONSTANT:
      stream << "const " << get_tyty_name (place.tyty);
      break;
    case Place::INVALID:
      stream << "_INVALID";
    }
}

void
Dump::visit_move_place (PlaceId place_id)
{
  const Place &place = func.place_db[place_id];
  if (place.is_rvalue || !place.is_copy)
    stream << "move ";
  visit_place (place_id);
}

void
Dump::visit (BorrowExpr &expr)
{
  stream << "&";
  visit_lifetime (node_place);
  visit_place (expr.get_place ());
}

void
Dump::visit_lifetime (PlaceId place_id)
{
  const Place &place = func.place_db[place_id];
  if (place.lifetime.has_lifetime ())
    {
      if (place.lifetime.id == STATIC_LIFETIME_ID)
	stream << "'static ";
      else
	stream << "'#" << get_lifetime_name (place.lifetime) << " ";
    }
}

void
Dump::visit (InitializerExpr &expr)
{
  stream << "{";
  print_comma_separated (stream, expr.get_values (), [this] (PlaceId place_id) {
    visit_move_place (place_id);
  });
  stream << "}";
}

void
Dump::visit (CallExpr &expr)
{
  stream << "Call(";
  if (auto fn_type
      = func.place_db[expr.get_callable ()].tyty->try_as<TyTy::FnType> ())
    {
      stream << fn_type->get_identifier ();
    }
  else
    {
      visit_move_place (expr.get_callable ());
    }
  stream << ")(";
  print_comma_separated (stream, expr.get_arguments (),
			 [this] (PlaceId place_id) {
			   visit_move_place (place_id);
			 });
  stream << ") -> [";
  print_comma_separated (stream, func.basic_blocks[node_bb].successors,
			 [this] (BasicBlockId succ) {
			   stream << "bb" << bb_fold_map[succ];
			 });
  stream << "]";
  bb_terminated = true;
}

void
Dump::visit (Operator<1> &expr)
{
  stream << "Operator(";
  visit_move_place (expr.get_operand<0> ());
  stream << ")";
}

void
Dump::visit (Operator<2> &expr)
{
  stream << "Operator(";
  visit_move_place (expr.get_operand<0> ());
  stream << ", ";
  visit_move_place (expr.get_operand<1> ());
  stream << ")";
}
void
Dump::visit (Assignment &expr)
{
  visit_move_place (expr.get_rhs ());
}

} // namespace BIR
} // namespace Rust
