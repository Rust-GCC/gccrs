type A = [u8; 4]::AssocTy;
// { dg-error ".E0223." "" { target *-*-* } .-1 }
// { dg-error ".E0223." "" { target *-*-* } .-2 }

type B = [u8]::AssocTy;
// { dg-error ".E0223." "" { target *-*-* } .-1 }
// { dg-error ".E0223." "" { target *-*-* } .-2 }

type C = (u8)::AssocTy;
// { dg-error ".E0223." "" { target *-*-* } .-1 }
// { dg-error ".E0223." "" { target *-*-* } .-2 }

type D = (u8, u8)::AssocTy;
// { dg-error ".E0223." "" { target *-*-* } .-1 }
// { dg-error ".E0223." "" { target *-*-* } .-2 }

type E = _::AssocTy;
// { dg-error ".E0121." "" { target *-*-* } .-1 }
// { dg-error ".E0121." "" { target *-*-* } .-2 }

type F = &'static (u8)::AssocTy;
// { dg-error ".E0223." "" { target *-*-* } .-1 }
// { dg-error ".E0223." "" { target *-*-* } .-2 }

// Qualified paths cannot appear in bounds, so the recovery
// should apply to the whole sum and not `(Send)`.
type G = dyn 'static + (Send)::AssocTy;
// { dg-error ".E0223." "" { target *-*-* } .-1 }
// { dg-error ".E0223." "" { target *-*-* } .-2 }

// This is actually a legal path with fn-like generic arguments in the middle!
// Recovery should not apply in this context.
type H = Fn(u8) -> (u8)::Output;
// { dg-error ".E0223." "" { target *-*-* } .-1 }

macro_rules! ty {
    ($ty: ty) => ($ty::AssocTy);
// { dg-error ".E0223." "" { target *-*-* } .-1 }
// { dg-error ".E0223." "" { target *-*-* } .-2 }
    () => (u8);
}

type J = ty!(u8);
type I = ty!()::AssocTy;
// { dg-error ".E0223." "" { target *-*-* } .-1 }
// { dg-error ".E0223." "" { target *-*-* } .-2 }

trait K<A, B> {}
fn foo<X: K<_, _>>(x: X) {}
// { dg-error ".E0121." "" { target *-*-* } .-1 }

fn bar<F>(_: F) where F: Fn() -> _ {}
// { dg-error ".E0121." "" { target *-*-* } .-1 }

fn baz<F: Fn() -> _>(_: F) {}
// { dg-error ".E0121." "" { target *-*-* } .-1 }

struct L<F>(F) where F: Fn() -> _;
// { dg-error ".E0121." "" { target *-*-* } .-1 }
struct M<F> where F: Fn() -> _ {
// { dg-error ".E0121." "" { target *-*-* } .-1 }
    a: F,
}
enum N<F> where F: Fn() -> _ {
// { dg-error ".E0121." "" { target *-*-* } .-1 }
    Foo(F),
}

union O<F> where F: Fn() -> _ {
// { dg-error ".E0121." "" { target *-*-* } .-1 }
    foo: F,
}

trait P<F> where F: Fn() -> _ {
// { dg-error ".E0121." "" { target *-*-* } .-1 }
}

trait Q {
    fn foo<F>(_: F) where F: Fn() -> _ {}
// { dg-error ".E0121." "" { target *-*-* } .-1 }
}

fn main() {}

