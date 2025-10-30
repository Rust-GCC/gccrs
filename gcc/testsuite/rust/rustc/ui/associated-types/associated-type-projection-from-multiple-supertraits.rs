// Test equality constraints in a where clause where the type being
// equated appears in a supertrait.

pub trait Vehicle {
    type Color;

    fn go(&self) {  }
}

pub trait Box {
    type Color;
    //
    fn mail(&self) {  }
}

pub trait BoxCar : Box + Vehicle {
}

fn dent<C:BoxCar>(c: C, color: C::Color) {
// { dg-error ".E0221." "" { target *-*-* } .-1 }
}

fn dent_object<COLOR>(c: dyn BoxCar<Color=COLOR>) {
// { dg-error ".E0191." "" { target *-*-* } .-1 }
// { dg-error ".E0191." "" { target *-*-* } .-2 }
}

fn paint<C:BoxCar>(c: C, d: C::Color) {
// { dg-error ".E0221." "" { target *-*-* } .-1 }
}

fn dent_object_2<COLOR>(c: dyn BoxCar) where <dyn BoxCar as Vehicle>::Color = COLOR {
// { dg-error ".E0191." "" { target *-*-* } .-1 }
// { dg-error ".E0191." "" { target *-*-* } .-2 }
}

fn dent_object_3<X, COLOR>(c: X)
where X: BoxCar,
    X: Vehicle<Color = COLOR>,
    X: Box<Color = COLOR>
{} // OK!

pub fn main() { }

