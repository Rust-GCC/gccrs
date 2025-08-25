#![feature(const_generics)]
#![allow(incomplete_features)]

#[derive(PartialEq, Eq)]
enum CompileFlag {
  A,
  B,
}

pub fn test_1<const CF: CompileFlag>() {}
pub fn test_2<T, const CF: CompileFlag>(x: T) {}
pub struct Example<const CF: CompileFlag, T=u32>{
  x: T,
}

impl<const CF: CompileFlag, T> Example<CF, T> {
  const ASSOC_FLAG: CompileFlag = CompileFlag::A;
}

pub fn main() {
  test_1::<CompileFlag::A>();
// { dg-error ".E0107." "" { target *-*-* } .-1 }
// { dg-error ".E0107." "" { target *-*-* } .-2 }
// { dg-error ".E0107." "" { target *-*-* } .-3 }

  test_2::<_, CompileFlag::A>(0);
// { dg-error ".E0107." "" { target *-*-* } .-1 }
// { dg-error ".E0107." "" { target *-*-* } .-2 }
// { dg-error ".E0107." "" { target *-*-* } .-3 }

  let _: Example<CompileFlag::A, _> = Example { x: 0 };
// { dg-error ".E0107." "" { target *-*-* } .-1 }
// { dg-error ".E0107." "" { target *-*-* } .-2 }
// { dg-error ".E0107." "" { target *-*-* } .-3 }

  let _: Example<Example::ASSOC_FLAG, _> = Example { x: 0 };
// { dg-error ".E0107." "" { target *-*-* } .-1 }
// { dg-error ".E0107." "" { target *-*-* } .-2 }
}

