#ifndef RUST_COMPILE_DROP
#define RUST_COMPILE_DROP

#include "rust-system.h"
#include "rust-hir-map.h"

class Bvariable;

namespace Rust {

namespace TyTy {
class BaseType;
}

namespace Compile {

class Context;

struct DropCandidate
{
  DropCandidate (HirId hirid, location_t locus) : hirid (hirid), locus (locus)
  {}

  HirId hirid;
  location_t locus;
};

class CompileDrop
{
public:
  static bool type_has_drop_impl (Context *ctx, TyTy::BaseType *ty);

  static tree compile_drop_call (Context *ctx, Bvariable *var,
				 TyTy::BaseType *ty, location_t locus);

  static void emit_current_scope_drop_calls (Context *ctx);
};

} // namespace Compile
} // namespace Rust

#endif // RUST_COMPILE_DROP