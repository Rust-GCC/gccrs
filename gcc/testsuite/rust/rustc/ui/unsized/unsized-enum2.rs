use std::ops::Deref;

// Due to aggressive error message deduplication, we require 20 *different*
// unsized types (even Path and [u8] are considered the "same").

trait Foo {}
trait Bar {}
trait FooBar {}
trait BarFoo {}

trait PathHelper1 {}
trait PathHelper2 {}
trait PathHelper3 {}
trait PathHelper4 {}

struct Path1(dyn PathHelper1);
struct Path2(dyn PathHelper2);
struct Path3(dyn PathHelper3);
struct Path4(dyn PathHelper4);

enum E<W: ?Sized, X: ?Sized, Y: ?Sized, Z: ?Sized> {
    // parameter
    VA(W),
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    VB{x: X},
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    VC(isize, Y),
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    VD{u: isize, x: Z},
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    // slice / str
    VE([u8]),
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    VF{x: str},
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    VG(isize, [f32]),
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    VH{u: isize, x: [u32]},
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    // unsized struct
    VI(Path1),
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    VJ{x: Path2},
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    VK(isize, Path3),
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    VL{u: isize, x: Path4},
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    // plain trait
    VM(dyn Foo),
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    VN{x: dyn Bar},
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    VO(isize, dyn FooBar),
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    VP{u: isize, x: dyn BarFoo},
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    // projected
    VQ(<&'static [i8] as Deref>::Target),
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    VR{x: <&'static [char] as Deref>::Target},
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    VS(isize, <&'static [f64] as Deref>::Target),
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    VT{u: isize, x: <&'static [i32] as Deref>::Target},
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}


fn main() { }

