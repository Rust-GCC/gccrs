// This test ensures that attributes on formals in generic parameter
// lists are included when we are checking for unstable attributes.

struct StLt<#[lt_struct] 'a>(&'a u32);
// { dg-error "" "" { target *-*-* } .-1 }
struct StTy<#[ty_struct] I>(I);
// { dg-error "" "" { target *-*-* } .-1 }

enum EnLt<#[lt_enum] 'b> { A(&'b u32), B }
// { dg-error "" "" { target *-*-* } .-1 }
enum EnTy<#[ty_enum] J> { A(J), B }
// { dg-error "" "" { target *-*-* } .-1 }

trait TrLt<#[lt_trait] 'c> { fn foo(&self, _: &'c [u32]) -> &'c u32; }
// { dg-error "" "" { target *-*-* } .-1 }
trait TrTy<#[ty_trait] K> { fn foo(&self, _: K); }
// { dg-error "" "" { target *-*-* } .-1 }

type TyLt<#[lt_type] 'd> = &'d u32;
// { dg-error "" "" { target *-*-* } .-1 }
type TyTy<#[ty_type] L> = (L, );
// { dg-error "" "" { target *-*-* } .-1 }

impl<#[lt_inherent] 'e> StLt<'e> { }
// { dg-error "" "" { target *-*-* } .-1 }
impl<#[ty_inherent] M> StTy<M> { }
// { dg-error "" "" { target *-*-* } .-1 }

impl<#[lt_impl_for] 'f> TrLt<'f> for StLt<'f> {
// { dg-error "" "" { target *-*-* } .-1 }
    fn foo(&self, _: &'f [u32]) -> &'f u32 { loop { } }
}
impl<#[ty_impl_for] N> TrTy<N> for StTy<N> {
// { dg-error "" "" { target *-*-* } .-1 }
    fn foo(&self, _: N) { }
}

fn f_lt<#[lt_fn] 'g>(_: &'g [u32]) -> &'g u32 { loop { } }
// { dg-error "" "" { target *-*-* } .-1 }
fn f_ty<#[ty_fn] O>(_: O) { }
// { dg-error "" "" { target *-*-* } .-1 }

impl<I> StTy<I> {
    fn m_lt<#[lt_meth] 'h>(_: &'h [u32]) -> &'h u32 { loop { } }
// { dg-error "" "" { target *-*-* } .-1 }
    fn m_ty<#[ty_meth] P>(_: P) { }
// { dg-error "" "" { target *-*-* } .-1 }
}

fn hof_lt<Q>(_: Q)
    where Q: for <#[lt_hof] 'i> Fn(&'i [u32]) -> &'i u32
// { dg-error "" "" { target *-*-* } .-1 }
{
}

fn main() {

}

