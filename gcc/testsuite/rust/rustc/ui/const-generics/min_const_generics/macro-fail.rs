#![feature(min_const_generics)]

struct Example<const N: usize>;

macro_rules! external_macro {
  () => {{
// { dg-error "" "" { target *-*-* } .-1 }
    const X: usize = 1337;
    X
  }}
}

trait Marker<const N: usize> {}
impl<const N: usize> Marker<N> for Example<N> {}

fn make_marker() -> impl Marker<gimme_a_const!(marker)> {
// { dg-error ".E0107." "" { target *-*-* } .-1 }
// { dg-error ".E0107." "" { target *-*-* } .-2 }
  Example::<gimme_a_const!(marker)>
// { dg-error ".E0107." "" { target *-*-* } .-1 }
// { dg-error ".E0107." "" { target *-*-* } .-2 }
}

fn from_marker(_: impl Marker<{
    #[macro_export]
    macro_rules! inline { () => {{ 3 }} }; inline!()
}>) {}

fn main() {
  let _ok = Example::<{
    #[macro_export]
    macro_rules! gimme_a_const {
      ($rusty: ident) => {{ let $rusty = 3; *&$rusty }}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    };
    gimme_a_const!(run)
  }>;

  let _fail = Example::<external_macro!()>;
// { dg-error ".E0107." "" { target *-*-* } .-1 }
// { dg-error ".E0107." "" { target *-*-* } .-2 }

  let _fail = Example::<gimme_a_const!()>;
// { dg-error ".E0107." "" { target *-*-* } .-1 }
// { dg-error ".E0107." "" { target *-*-* } .-2 }
// { dg-error ".E0107." "" { target *-*-* } .-3 }
}

