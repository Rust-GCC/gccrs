#include "rust-compile-drop.h"
#include "rust-compile-base.h"
#include "rust-compile-context.h"
#include "rust-compile-implitem.h"
#include "rust-hir-path-probe.h"
#include "rust-hir-trait-reference.h"
#include "rust-hir-type-bounds.h"
#include "rust-lang-item.h"
#include "rust-tyty.h"

namespace Rust {
namespace Compile {

bool
CompileDrop::type_has_drop_impl (Context *ctx, TyTy::BaseType *ty)
{
  auto drop_lang_item
    = ctx->get_mappings ().lookup_lang_item (LangItem::Kind::DROP);

  if (!drop_lang_item.has_value ())
    return false;

  DefId drop_id = drop_lang_item.value ();

  auto candidates = Resolver::TypeBoundsProbe::Probe (ty);
  for (auto &candidate : candidates)
    {
      Resolver::TraitReference *trait_ref = candidate.first;
      if (trait_ref != nullptr && trait_ref->get_defid () == drop_id)
	return true;
    }

  return false;
}

// Find the Drop trait, look for the drop method, and build the function call.
tree
CompileDrop::compile_drop_call (Context *ctx, Bvariable *var,
				TyTy::BaseType *ty, location_t locus)
{
  auto drop_lang = ctx->get_mappings ().lookup_lang_item (LangItem::Kind::DROP);
  if (!drop_lang.has_value ())
    return NULL_TREE;

  Resolver::TraitReference *drop_ref = nullptr;
  bool ok
    = ctx->get_tyctx ()->lookup_trait_reference (drop_lang.value (), &drop_ref);
  if (!ok)
    return NULL_TREE;

  HIR::PathIdentSegment segment ("drop");
  auto candidates
    = Resolver::PathProbeImplTrait::Probe (ty->get_root (), segment, drop_ref);

  for (auto &candidate : candidates)
    {
      if (!candidate.is_impl_candidate ()
	  || candidate.ty->get_kind () != TyTy::TypeKind::FNDEF)
	continue;

      auto *fn_type = static_cast<TyTy::FnType *> (candidate.ty);
      tree fn_addr
	= CompileInherentImplItem::Compile (candidate.item.impl.impl_item, ctx,
					    fn_type, locus);

      tree var_expr = Backend::var_expression (var, locus);
      tree var_addr = HIRCompileBase::address_expression (var_expr, locus);

      return Backend::call_expression (fn_addr, {var_addr}, nullptr, locus);
    }
  return NULL_TREE;
}

void
CompileDrop::emit_current_scope_drop_calls (Context *ctx)
{
  auto &drop_candidates = ctx->peek_block_drop_candidates ();

  for (auto it = drop_candidates.rbegin (); it != drop_candidates.rend (); ++it)
    {
      TyTy::BaseType *ty = nullptr;
      Bvariable *var = nullptr;

      bool ok = ctx->get_tyctx ()->lookup_type (it->hirid, &ty);
      rust_assert (ok);

      ok = ctx->lookup_var_decl (it->hirid, &var);
      rust_assert (ok);

      tree drop_call = CompileDrop::compile_drop_call (ctx, var, ty, it->locus);
      if (drop_call != NULL_TREE)
	ctx->add_statement (convert_to_void (drop_call, ICV_STATEMENT));
    }
}

} // namespace Compile
} // namespace Rust
