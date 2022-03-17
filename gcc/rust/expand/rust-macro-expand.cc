// Copyright (C) 2020-2022 Free Software Foundation, Inc.

// This file is part of GCC.

// GCC is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 3, or (at your option) any later
// version.

// GCC is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.

// You should have received a copy of the GNU General Public License
// along with GCC; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

#include "rust-macro-expand.h"
#include "rust-macro-substitute-ctx.h"
#include "rust-ast-full.h"
#include "rust-ast-visitor.h"
#include "rust-diagnostics.h"
#include "rust-parse.h"
#include "rust-attribute-visitor.h"

namespace Rust {
void
MacroExpander::parse_macro_to_meta_item (AST::MacroInvocData &invoc)
{
  // only parse if not already parsed
  if (invoc.is_parsed ())
    return;

  std::unique_ptr<AST::AttrInputMetaItemContainer> converted_input (
    invoc.get_delim_tok_tree ().parse_to_meta_item ());

  if (converted_input == nullptr)
    {
      rust_debug ("DEBUG: failed to parse macro to meta item");
      // TODO: do something now? is this an actual error?
    }
  else
    {
      std::vector<std::unique_ptr<AST::MetaItemInner>> meta_items (
	std::move (converted_input->get_items ()));
      invoc.set_meta_item_output (std::move (meta_items));
    }
}

AST::Literal
MacroExpander::expand_cfg_macro (AST::MacroInvocData &invoc)
{
  // only allow on cfg macros
  if (invoc.get_path () != "cfg")
    return AST::Literal::create_error ();

  parse_macro_to_meta_item (invoc);

  /* TODO: assuming that cfg! macros can only have one meta item inner, like cfg
   * attributes */
  if (invoc.get_meta_items ().size () != 1)
    return AST::Literal::create_error ();

  bool result = invoc.get_meta_items ()[0]->check_cfg_predicate (session);
  if (result)
    return AST::Literal ("true", AST::Literal::BOOL, CORETYPE_BOOL);
  else
    return AST::Literal ("false", AST::Literal::BOOL, CORETYPE_BOOL);
}

AST::ASTFragment
MacroExpander::expand_decl_macro (Location invoc_locus,
				  AST::MacroInvocData &invoc,
				  AST::MacroRulesDefinition &rules_def,
				  bool semicolon)
{
  // ensure that both invocation and rules are in a valid state
  rust_assert (!invoc.is_marked_for_strip ());
  rust_assert (!rules_def.is_marked_for_strip ());
  rust_assert (rules_def.get_macro_rules ().size () > 0);

  /* probably something here about parsing invoc and rules def token trees to
   * token stream. if not, how would parser handle the captures of exprs and
   * stuff? on the other hand, token trees may be kind of useful in rules def as
   * creating a point where recursion can occur (like having
   * "compare_macro_match" and then it calling itself when it finds delimiters)
   */

  /* find matching rule to invoc token tree, based on macro rule's matcher. if
   * none exist, error.
   * - specifically, check each matcher in order. if one fails to match, move
   * onto next. */
  /* TODO: does doing this require parsing expressions and whatever in the
   * invoc? if so, might as well save the results if referenced using $ or
   * whatever. If not, do another pass saving them. Except this is probably
   * useless as different rules could have different starting points for exprs
   * or whatever. Decision trees could avoid this, but they have their own
   * issues. */
  /* TODO: will need to modify the parser so that it can essentially "catch"
   * errors - maybe "try_parse_expr" or whatever methods. */
  // this technically creates a back-tracking parser - this will be the
  // implementation style

  /* then, after results are saved, generate the macro output from the
   * transcriber token tree. if i understand this correctly, the macro
   * invocation gets replaced by the transcriber tokens, except with
   * substitutions made (e.g. for $i variables) */

  /* TODO: it is probably better to modify AST::Token to store a pointer to a
   * Lexer::Token (rather than being converted) - i.e. not so much have
   * AST::Token as a Token but rather a TokenContainer (as it is another type of
   * TokenTree). This will prevent re-conversion of Tokens between each type
   * all the time, while still allowing the heterogenous storage of token trees.
   */

  AST::DelimTokenTree &invoc_token_tree = invoc.get_delim_tok_tree ();

  // find matching arm
  AST::MacroRule *matched_rule = nullptr;
  std::map<std::string, MatchedFragmentContainer> matched_fragments;
  for (auto &rule : rules_def.get_rules ())
    {
      sub_stack.push ();
      bool did_match_rule = try_match_rule (rule, invoc_token_tree);
      matched_fragments = sub_stack.pop ();

      if (did_match_rule)
	{
	  // Debugging
	  //   for (auto &kv : matched_fragments)
	  //     rust_debug ("[fragment]: %s (%ld)", kv.first.c_str (),
	  // 		kv.second.get_fragments ().size ());

	  matched_rule = &rule;
	  break;
	}
    }

  if (matched_rule == nullptr)
    {
      RichLocation r (invoc_locus);
      r.add_range (rules_def.get_locus ());
      rust_error_at (r, "Failed to match any rule within macro");
      return AST::ASTFragment::create_error ();
    }

  return transcribe_rule (*matched_rule, invoc_token_tree, matched_fragments,
			  semicolon, peek_context ());
}

void
MacroExpander::expand_invoc (AST::MacroInvocation &invoc)
{
  if (depth_exceeds_recursion_limit ())
    {
      rust_error_at (invoc.get_locus (), "reached recursion limit");
      return;
    }

  AST::MacroInvocData &invoc_data = invoc.get_invoc_data ();

  // ??
  // switch on type of macro:
  //  - '!' syntax macro (inner switch)
  //      - procedural macro - "A token-based function-like macro"
  //      - 'macro_rules' (by example/pattern-match) macro? or not? "an
  // AST-based function-like macro"
  //      - else is unreachable
  //  - attribute syntax macro (inner switch)
  //  - procedural macro attribute syntax - "A token-based attribute
  // macro"
  //      - legacy macro attribute syntax? - "an AST-based attribute macro"
  //      - non-macro attribute: mark known
  //      - else is unreachable
  //  - derive macro (inner switch)
  //      - derive or legacy derive - "token-based" vs "AST-based"
  //      - else is unreachable
  //  - derive container macro - unreachable

  // lookup the rules for this macro
  NodeId resolved_node = UNKNOWN_NODEID;
  bool found = resolver->get_macro_scope ().lookup (
    Resolver::CanonicalPath::new_seg (invoc.get_pattern_node_id (),
				      invoc_data.get_path ().as_string ()),
    &resolved_node);
  if (!found)
    {
      rust_error_at (invoc.get_locus (), "unknown macro");
      return;
    }

  // lookup the rules
  AST::MacroRulesDefinition *rules_def = nullptr;
  bool ok = mappings->lookup_macro_def (resolved_node, &rules_def);
  rust_assert (ok);

  auto fragment = AST::ASTFragment::create_empty ();

  if (rules_def->is_builtin ())
    fragment
      = rules_def->get_builtin_transcriber () (invoc.get_locus (), invoc_data);
  else
    fragment
      = expand_decl_macro (invoc.get_locus (), invoc_data, *rules_def, false);

  set_expanded_fragment (std::move (fragment));
}

// FIXME: Arthur: Refactor these two functions, they're really similar
void
MacroExpander::expand_invoc_semi (AST::MacroInvocation &invoc)
{
  if (depth_exceeds_recursion_limit ())
    {
      rust_error_at (invoc.get_locus (), "reached recursion limit");
      return;
    }

  AST::MacroInvocData &invoc_data = invoc.get_invoc_data ();

  // lookup the rules for this macro
  NodeId resolved_node = UNKNOWN_NODEID;
  bool found = resolver->get_macro_scope ().lookup (
    Resolver::CanonicalPath::new_seg (invoc.get_macro_node_id (),
				      invoc_data.get_path ().as_string ()),
    &resolved_node);
  if (!found)
    {
      rust_error_at (invoc.get_locus (), "unknown macro");
      return;
    }

  // lookup the rules
  AST::MacroRulesDefinition *rules_def = nullptr;
  bool ok = mappings->lookup_macro_def (resolved_node, &rules_def);
  rust_assert (ok);

  auto fragment = AST::ASTFragment::create_empty ();

  if (rules_def->is_builtin ())
    fragment
      = rules_def->get_builtin_transcriber () (invoc.get_locus (), invoc_data);
  else
    fragment
      = expand_decl_macro (invoc.get_locus (), invoc_data, *rules_def, true);

  set_expanded_fragment (std::move (fragment));
}

/* Determines whether any cfg predicate is false and hence item with attributes
 * should be stripped. Note that attributes must be expanded before calling. */
bool
MacroExpander::fails_cfg (const AST::AttrVec &attrs) const
{
  for (const auto &attr : attrs)
    {
      if (attr.get_path () == "cfg" && !attr.check_cfg_predicate (session))
	return true;
    }
  return false;
}

/* Determines whether any cfg predicate is false and hence item with attributes
 * should be stripped. Will expand attributes as well. */
bool
MacroExpander::fails_cfg_with_expand (AST::AttrVec &attrs) const
{
  // TODO: maybe have something that strips cfg attributes that evaluate true?
  for (auto &attr : attrs)
    {
      if (attr.get_path () == "cfg")
	{
	  if (!attr.is_parsed_to_meta_item ())
	    attr.parse_attr_to_meta_item ();

	  // DEBUG
	  if (!attr.is_parsed_to_meta_item ())
	    rust_debug ("failed to parse attr to meta item, right before "
			"cfg predicate check");
	  else
	    rust_debug ("attr has been successfully parsed to meta item, "
			"right before cfg predicate check");

	  if (!attr.check_cfg_predicate (session))
	    {
	      // DEBUG
	      rust_debug (
		"cfg predicate failed for attribute: \033[0;31m'%s'\033[0m",
		attr.as_string ().c_str ());

	      return true;
	    }
	  else
	    {
	      // DEBUG
	      rust_debug ("cfg predicate succeeded for attribute: "
			  "\033[0;31m'%s'\033[0m",
			  attr.as_string ().c_str ());
	    }
	}
    }
  return false;
}

// Expands cfg_attr attributes.
void
MacroExpander::expand_cfg_attrs (AST::AttrVec &attrs)
{
  for (std::size_t i = 0; i < attrs.size (); i++)
    {
      auto &attr = attrs[i];
      if (attr.get_path () == "cfg_attr")
	{
	  if (!attr.is_parsed_to_meta_item ())
	    attr.parse_attr_to_meta_item ();

	  if (attr.check_cfg_predicate (session))
	    {
	      // split off cfg_attr
	      AST::AttrVec new_attrs = attr.separate_cfg_attrs ();

	      // remove attr from vector
	      attrs.erase (attrs.begin () + i);

	      // add new attrs to vector
	      attrs.insert (attrs.begin () + i,
			    std::make_move_iterator (new_attrs.begin ()),
			    std::make_move_iterator (new_attrs.end ()));
	    }

	  /* do something - if feature (first token in tree) is in fact enabled,
	   * make tokens listed afterwards into attributes. i.e.: for
	   * [cfg_attr(feature = "wow", wow1, wow2)], if "wow" is true, then add
	   * attributes [wow1] and [wow2] to attribute list. This can also be
	   * recursive, so check for expanded attributes being recursive and
	   * possibly recursively call the expand_attrs? */
	}
      else
	{
	  i++;
	}
    }
  attrs.shrink_to_fit ();
}

void
MacroExpander::expand_crate ()
{
  NodeId scope_node_id = crate.get_node_id ();
  resolver->get_macro_scope ().push (scope_node_id);

  /* fill macro/decorator map from init list? not sure where init list comes
   * from? */

  // TODO: does cfg apply for inner attributes? research.
  // the apparent answer (from playground test) is yes

  // expand crate cfg_attr attributes
  expand_cfg_attrs (crate.inner_attrs);

  if (fails_cfg_with_expand (crate.inner_attrs))
    {
      // basically, delete whole crate
      crate.strip_crate ();
      // TODO: maybe create warning here? probably not desired behaviour
    }
  // expand module attributes?

  push_context (ITEM);

  // expand attributes recursively and strip items if required
  AttrVisitor attr_visitor (*this);
  auto &items = crate.items;
  for (auto it = items.begin (); it != items.end ();)
    {
      auto &item = *it;

      // mark for stripping if required
      item->accept_vis (attr_visitor);

      auto fragment = take_expanded_fragment (attr_visitor);
      if (fragment.should_expand ())
	{
	  // Remove the current expanded invocation
	  it = items.erase (it);
	  for (auto &node : fragment.get_nodes ())
	    {
	      it = items.insert (it, node.take_item ());
	      it++;
	    }
	}
      else if (item->is_marked_for_strip ())
	it = items.erase (it);
      else
	it++;
    }

  pop_context ();

  // TODO: should recursive attribute and macro expansion be done in the same
  // transversal? Or in separate ones like currently?

  // expand module tree recursively

  // post-process

  // extract exported macros?
}

bool
MacroExpander::depth_exceeds_recursion_limit () const
{
  return expansion_depth >= cfg.recursion_limit;
}

bool
MacroExpander::try_match_rule (AST::MacroRule &match_rule,
			       AST::DelimTokenTree &invoc_token_tree)
{
  MacroInvocLexer lex (invoc_token_tree.to_token_stream ());
  Parser<MacroInvocLexer> parser (std::move (lex));

  AST::MacroMatcher &matcher = match_rule.get_matcher ();

  expansion_depth++;
  if (!match_matcher (parser, matcher))
    {
      expansion_depth--;
      return false;
    }
  expansion_depth--;

  bool used_all_input_tokens = parser.skip_token (END_OF_FILE);
  return used_all_input_tokens;
}

bool
MacroExpander::match_fragment (Parser<MacroInvocLexer> &parser,
			       AST::MacroMatchFragment &fragment)
{
  switch (fragment.get_frag_spec ())
    {
    case AST::MacroFragSpec::EXPR:
      parser.parse_expr ();
      break;

    case AST::MacroFragSpec::BLOCK:
      parser.parse_block_expr ();
      break;

    case AST::MacroFragSpec::IDENT:
      parser.parse_identifier_pattern ();
      break;

    case AST::MacroFragSpec::LITERAL:
      parser.parse_literal_expr ();
      break;

    case AST::MacroFragSpec::ITEM:
      parser.parse_item (false);
      break;

    case AST::MacroFragSpec::TY:
      parser.parse_type ();
      break;

    case AST::MacroFragSpec::PAT:
      parser.parse_pattern ();
      break;

    case AST::MacroFragSpec::PATH:
      parser.parse_path_in_expression ();
      break;

    case AST::MacroFragSpec::VIS:
      parser.parse_visibility ();
      break;

    case AST::MacroFragSpec::STMT:
      parser.parse_stmt ();
      break;

    case AST::MacroFragSpec::LIFETIME:
      parser.parse_lifetime_params ();
      break;

      // is meta attributes?
    case AST::MacroFragSpec::META:
      // parser.parse_inner_attribute ?
      // parser.parse_outer_attribute ?
      // parser.parse_attribute_body ?
      // parser.parse_doc_comment ?
      gcc_unreachable ();
      break;

      // what is TT?
    case AST::MacroFragSpec::TT:
      // parser.parse_token_tree() ?
      gcc_unreachable ();
      break;

      // i guess we just ignore invalid and just error out
    case AST::MacroFragSpec::INVALID:
      return false;
    }

  // it matches if the parser did not produce errors trying to parse that type
  // of item
  return !parser.has_errors ();
}

bool
MacroExpander::match_matcher (Parser<MacroInvocLexer> &parser,
			      AST::MacroMatcher &matcher)
{
  if (depth_exceeds_recursion_limit ())
    {
      rust_error_at (matcher.get_match_locus (), "reached recursion limit");
      return false;
    }

  auto delimiter = parser.peek_current_token ();

  // this is used so we can check that we delimit the stream correctly.
  switch (delimiter->get_id ())
    {
      case LEFT_PAREN: {
	if (!parser.skip_token (LEFT_PAREN))
	  return false;
      }
      break;

      case LEFT_SQUARE: {
	if (!parser.skip_token (LEFT_SQUARE))
	  return false;
      }
      break;

      case LEFT_CURLY: {
	if (!parser.skip_token (LEFT_CURLY))
	  return false;
      }
      break;
    default:
      gcc_unreachable ();
    }

  const MacroInvocLexer &source = parser.get_token_source ();

  for (auto &match : matcher.get_matches ())
    {
      size_t offs_begin = source.get_offs ();

      switch (match->get_macro_match_type ())
	{
	  case AST::MacroMatch::MacroMatchType::Fragment: {
	    AST::MacroMatchFragment *fragment
	      = static_cast<AST::MacroMatchFragment *> (match.get ());
	    if (!match_fragment (parser, *fragment))
	      return false;

	    // matched fragment get the offset in the token stream
	    size_t offs_end = source.get_offs ();
	    sub_stack.insert_fragment (
	      MatchedFragment (fragment->get_ident (), offs_begin, offs_end));
	  }
	  break;

	  case AST::MacroMatch::MacroMatchType::Tok: {
	    AST::Token *tok = static_cast<AST::Token *> (match.get ());
	    if (!match_token (parser, *tok))
	      return false;
	  }
	  break;

	  case AST::MacroMatch::MacroMatchType::Repetition: {
	    AST::MacroMatchRepetition *rep
	      = static_cast<AST::MacroMatchRepetition *> (match.get ());
	    if (!match_repetition (parser, *rep))
	      return false;
	  }
	  break;

	  case AST::MacroMatch::MacroMatchType::Matcher: {
	    AST::MacroMatcher *m
	      = static_cast<AST::MacroMatcher *> (match.get ());
	    expansion_depth++;
	    if (!match_matcher (parser, *m))
	      {
		expansion_depth--;
		return false;
	      }
	    expansion_depth--;
	  }
	  break;
	}
    }

  switch (delimiter->get_id ())
    {
      case LEFT_PAREN: {
	if (!parser.skip_token (RIGHT_PAREN))
	  return false;
      }
      break;

      case LEFT_SQUARE: {
	if (!parser.skip_token (RIGHT_SQUARE))
	  return false;
      }
      break;

      case LEFT_CURLY: {
	if (!parser.skip_token (RIGHT_CURLY))
	  return false;
      }
      break;
    default:
      gcc_unreachable ();
    }

  return true;
}

bool
MacroExpander::match_token (Parser<MacroInvocLexer> &parser, AST::Token &token)
{
  // FIXME this needs to actually match the content and the type
  return parser.skip_token (token.get_id ());
}

bool
MacroExpander::match_n_matches (Parser<MacroInvocLexer> &parser,
				AST::MacroMatchRepetition &rep,
				size_t &match_amount, size_t lo_bound,
				size_t hi_bound)
{
  match_amount = 0;
  auto &matches = rep.get_matches ();

  const MacroInvocLexer &source = parser.get_token_source ();
  while (true)
    {
      // If the current token is a closing macro delimiter, break away.
      // TODO: Is this correct?
      auto t_id = parser.peek_current_token ()->get_id ();
      if (t_id == RIGHT_PAREN || t_id == RIGHT_SQUARE || t_id == RIGHT_CURLY)
	break;

      // Skip parsing a separator on the first match, otherwise consume it.
      // If it isn't present, this is an error
      if (rep.has_sep () && match_amount > 0)
	if (!match_token (parser, *rep.get_sep ()))
	  break;

      bool valid_current_match = false;
      for (auto &match : matches)
	{
	  size_t offs_begin = source.get_offs ();
	  switch (match->get_macro_match_type ())
	    {
	      case AST::MacroMatch::MacroMatchType::Fragment: {
		AST::MacroMatchFragment *fragment
		  = static_cast<AST::MacroMatchFragment *> (match.get ());
		valid_current_match = match_fragment (parser, *fragment);

		// matched fragment get the offset in the token stream
		size_t offs_end = source.get_offs ();
		sub_stack.insert_fragment (
		  MatchedFragment (fragment->get_ident (), offs_begin,
				   offs_end));
	      }
	      break;

	      case AST::MacroMatch::MacroMatchType::Tok: {
		AST::Token *tok = static_cast<AST::Token *> (match.get ());
		valid_current_match = match_token (parser, *tok);
	      }
	      break;

	      case AST::MacroMatch::MacroMatchType::Repetition: {
		AST::MacroMatchRepetition *rep
		  = static_cast<AST::MacroMatchRepetition *> (match.get ());
		valid_current_match = match_repetition (parser, *rep);
	      }
	      break;

	      case AST::MacroMatch::MacroMatchType::Matcher: {
		AST::MacroMatcher *m
		  = static_cast<AST::MacroMatcher *> (match.get ());
		valid_current_match = match_matcher (parser, *m);
	      }
	      break;
	    }
	}
      // If we've encountered an error once, stop trying to match more
      // repetitions
      if (!valid_current_match)
	break;

      match_amount++;

      // Break early if we notice there's too many expressions already
      if (hi_bound && match_amount > hi_bound)
	break;
    }

  // Check if the amount of matches we got is valid: Is it more than the lower
  // bound and less than the higher bound?
  bool did_meet_lo_bound = match_amount >= lo_bound;
  bool did_meet_hi_bound = hi_bound ? match_amount <= hi_bound : true;

  return did_meet_lo_bound && did_meet_hi_bound;
}

bool
MacroExpander::match_repetition (Parser<MacroInvocLexer> &parser,
				 AST::MacroMatchRepetition &rep)
{
  size_t match_amount = 0;
  bool res = false;

  std::string lo_str;
  std::string hi_str;
  switch (rep.get_op ())
    {
    case AST::MacroMatchRepetition::MacroRepOp::ANY:
      lo_str = "0";
      hi_str = "+inf";
      res = match_n_matches (parser, rep, match_amount);
      break;
    case AST::MacroMatchRepetition::MacroRepOp::ONE_OR_MORE:
      lo_str = "1";
      hi_str = "+inf";
      res = match_n_matches (parser, rep, match_amount, 1);
      break;
    case AST::MacroMatchRepetition::MacroRepOp::ZERO_OR_ONE:
      lo_str = "0";
      hi_str = "1";
      res = match_n_matches (parser, rep, match_amount, 0, 1);
      break;
    default:
      gcc_unreachable ();
    }

  if (!res)
    rust_error_at (rep.get_match_locus (),
		   "invalid amount of matches for macro invocation. Expected "
		   "between %s and %s, got %lu",
		   lo_str.c_str (), hi_str.c_str (), match_amount);

  rust_debug_loc (rep.get_match_locus (), "%s matched %lu times",
		  res ? "successfully" : "unsuccessfully", match_amount);

  // We have to handle zero fragments differently: They will not have been
  // "matched" but they are still valid and should be inserted as a special
  // case. So we go through the stack map, and for every fragment which doesn't
  // exist, insert a zero-matched fragment.
  auto &stack_map = sub_stack.peek ();
  for (auto &match : rep.get_matches ())
    {
      if (match->get_macro_match_type ()
	  == AST::MacroMatch::MacroMatchType::Fragment)
	{
	  auto fragment = static_cast<AST::MacroMatchFragment *> (match.get ());
	  auto it = stack_map.find (fragment->get_ident ());

	  if (it == stack_map.end ())
	    sub_stack.insert_matches (fragment->get_ident (),
				      MatchedFragmentContainer::zero ());
	}
    }

  return res;
}

/**
 * Helper function to refactor calling a parsing function 0 or more times
 */
static std::vector<AST::SingleASTNode>
parse_many (Parser<MacroInvocLexer> &parser, TokenId &delimiter,
	    std::function<AST::SingleASTNode ()> parse_fn)
{
  std::vector<AST::SingleASTNode> nodes;
  while (true)
    {
      if (parser.peek_current_token ()->get_id () == delimiter)
	break;

      auto node = parse_fn ();
      nodes.emplace_back (std::move (node));
    }

  return nodes;
}

/**
 * Transcribe 0 or more items from a macro invocation
 *
 * @param parser Parser to extract items from
 * @param delimiter Id of the token on which parsing should stop
 */
static std::vector<AST::SingleASTNode>
transcribe_many_items (Parser<MacroInvocLexer> &parser, TokenId &delimiter)
{
  return parse_many (parser, delimiter, [&parser] () {
    auto item = parser.parse_item (true);
    return AST::SingleASTNode (std::move (item));
  });
}

/**
 * Transcribe 0 or more external items from a macro invocation
 *
 * @param parser Parser to extract items from
 * @param delimiter Id of the token on which parsing should stop
 */
static std::vector<AST::SingleASTNode>
transcribe_many_ext (Parser<MacroInvocLexer> &parser, TokenId &delimiter)
{
  return parse_many (parser, delimiter, [&parser] () {
    auto item = parser.parse_external_item ();
    return AST::SingleASTNode (std::move (item));
  });
}

/**
 * Transcribe 0 or more trait items from a macro invocation
 *
 * @param parser Parser to extract items from
 * @param delimiter Id of the token on which parsing should stop
 */
static std::vector<AST::SingleASTNode>
transcribe_many_trait_items (Parser<MacroInvocLexer> &parser,
			     TokenId &delimiter)
{
  return parse_many (parser, delimiter, [&parser] () {
    auto item = parser.parse_trait_item ();
    return AST::SingleASTNode (std::move (item));
  });
}

/**
 * Transcribe 0 or more impl items from a macro invocation
 *
 * @param parser Parser to extract items from
 * @param delimiter Id of the token on which parsing should stop
 */
static std::vector<AST::SingleASTNode>
transcribe_many_impl_items (Parser<MacroInvocLexer> &parser, TokenId &delimiter)
{
  return parse_many (parser, delimiter, [&parser] () {
    auto item = parser.parse_inherent_impl_item ();
    return AST::SingleASTNode (std::move (item));
  });
}

/**
 * Transcribe 0 or more statements from a macro invocation
 *
 * @param parser Parser to extract statements from
 * @param delimiter Id of the token on which parsing should stop
 */
static std::vector<AST::SingleASTNode>
transcribe_many_stmts (Parser<MacroInvocLexer> &parser, TokenId &delimiter)
{
  // FIXME: This is invalid! It needs to also handle cases where the macro
  // transcriber is an expression, but since the macro call is followed by
  // a semicolon, it's a valid ExprStmt
  return parse_many (parser, delimiter, [&parser] () {
    auto stmt = parser.parse_stmt ();
    return AST::SingleASTNode (std::move (stmt));
  });
}

/**
 * Transcribe one expression from a macro invocation
 *
 * @param parser Parser to extract statements from
 */
static std::vector<AST::SingleASTNode>
transcribe_expression (Parser<MacroInvocLexer> &parser)
{
  auto expr = parser.parse_expr ();

  return {AST::SingleASTNode (std::move (expr))};
}

static std::vector<AST::SingleASTNode>
transcribe_on_delimiter (Parser<MacroInvocLexer> &parser, bool semicolon,
			 AST::DelimType delimiter, TokenId last_token_id)
{
  if (semicolon || delimiter == AST::DelimType::CURLY)
    return transcribe_many_stmts (parser, last_token_id);
  else
    return transcribe_expression (parser);
} // namespace Rust

static std::vector<AST::SingleASTNode>
transcribe_context (MacroExpander::ContextType ctx,
		    Parser<MacroInvocLexer> &parser, bool semicolon,
		    AST::DelimType delimiter, TokenId last_token_id)
{
  // The flow-chart in order to choose a parsing function is as follows:
  //
  // [switch special context]
  //     -- Item --> parser.parse_item();
  //     -- Trait --> parser.parse_trait_item();
  //     -- Impl --> parser.parse_impl_item();
  //     -- Extern --> parser.parse_extern_item();
  //     -- None --> [has semicolon?]
  //                 -- Yes --> parser.parse_stmt();
  //                 -- No --> [switch invocation.delimiter()]
  //                             -- { } --> parser.parse_stmt();
  //                             -- _ --> parser.parse_expr(); // once!

  // If there is a semicolon OR we are expanding a MacroInvocationSemi, then
  // we can parse multiple items. Otherwise, parse *one* expression

  switch (ctx)
    {
    case MacroExpander::ContextType::ITEM:
      return transcribe_many_items (parser, last_token_id);
      break;
    case MacroExpander::ContextType::TRAIT:
      return transcribe_many_trait_items (parser, last_token_id);
      break;
    case MacroExpander::ContextType::IMPL:
      return transcribe_many_impl_items (parser, last_token_id);
      break;
    case MacroExpander::ContextType::EXTERN:
      return transcribe_many_ext (parser, last_token_id);
      break;
    default:
      return transcribe_on_delimiter (parser, semicolon, delimiter,
				      last_token_id);
    }
}

AST::ASTFragment
MacroExpander::transcribe_rule (
  AST::MacroRule &match_rule, AST::DelimTokenTree &invoc_token_tree,
  std::map<std::string, MatchedFragmentContainer> &matched_fragments,
  bool semicolon, ContextType ctx)
{
  // we can manipulate the token tree to substitute the dollar identifiers so
  // that when we call parse its already substituted for us
  AST::MacroTranscriber &transcriber = match_rule.get_transcriber ();
  AST::DelimTokenTree &transcribe_tree = transcriber.get_token_tree ();

  auto invoc_stream = invoc_token_tree.to_token_stream ();
  auto macro_rule_tokens = transcribe_tree.to_token_stream ();

  auto substitute_context
    = SubstituteCtx (invoc_stream, macro_rule_tokens, matched_fragments);
  std::vector<std::unique_ptr<AST::Token>> substituted_tokens
    = substitute_context.substitute_tokens ();

  // handy for debugging
  // for (auto &tok : substituted_tokens)
  //   rust_debug ("[tok] %s", tok->as_string ().c_str ());

  // parse it to an ASTFragment
  MacroInvocLexer lex (std::move (substituted_tokens));
  Parser<MacroInvocLexer> parser (std::move (lex));

  auto last_token_id = TokenId::RIGHT_CURLY;

  // this is used so we can check that we delimit the stream correctly.
  switch (transcribe_tree.get_delim_type ())
    {
    case AST::DelimType::PARENS:
      last_token_id = TokenId::RIGHT_PAREN;
      rust_assert (parser.skip_token (LEFT_PAREN));
      break;

    case AST::DelimType::CURLY:
      rust_assert (parser.skip_token (LEFT_CURLY));
      break;

    case AST::DelimType::SQUARE:
      last_token_id = TokenId::RIGHT_SQUARE;
      rust_assert (parser.skip_token (LEFT_SQUARE));
      break;
    }

  // see https://github.com/Rust-GCC/gccrs/issues/22
  // TL;DR:
  //   - Treat all macro invocations with parentheses, (), or square brackets,
  //   [], as expressions.
  //   - If the macro invocation has curly brackets, {}, it may be parsed as a
  //   statement depending on the context.
  //   - If the macro invocation has a semicolon at the end, it must be parsed
  //   as a statement (either via ExpressionStatement or
  //   MacroInvocationWithSemi)

  auto nodes
    = transcribe_context (ctx, parser, semicolon,
			  invoc_token_tree.get_delim_type (), last_token_id);

  // emit any errors
  if (parser.has_errors ())
    {
      for (auto &err : parser.get_errors ())
	rust_error_at (err.locus, "%s", err.message.c_str ());
      return AST::ASTFragment::create_error ();
    }

  // are all the tokens used?
  bool did_delimit = parser.skip_token (last_token_id);

  bool reached_end_of_stream = did_delimit && parser.skip_token (END_OF_FILE);
  if (!reached_end_of_stream)
    {
      const_TokenPtr current_token = parser.peek_current_token ();
      rust_error_at (current_token->get_locus (),
		     "tokens here and after are unparsed");
    }

  return AST::ASTFragment (std::move (nodes));
}
} // namespace Rust
