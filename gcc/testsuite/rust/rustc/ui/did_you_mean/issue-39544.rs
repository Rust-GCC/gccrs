pub enum X {
    Y
}

pub struct Z {
    x: X
}

fn main() {
    let z = Z { x: X::Y };
    let _ = &mut z.x; // { dg-error ".E0596." "" { target *-*-* } }
}

impl Z {
    fn foo<'z>(&'z self) {
        let _ = &mut self.x; // { dg-error ".E0596." "" { target *-*-* } }
    }

    fn foo1(&self, other: &Z) {
        let _ = &mut self.x; // { dg-error ".E0596." "" { target *-*-* } }
        let _ = &mut other.x; // { dg-error ".E0596." "" { target *-*-* } }
    }

    fn foo2<'a>(&'a self, other: &Z) {
        let _ = &mut self.x; // { dg-error ".E0596." "" { target *-*-* } }
        let _ = &mut other.x; // { dg-error ".E0596." "" { target *-*-* } }
    }

    fn foo3<'a>(self: &'a Self, other: &Z) {
        let _ = &mut self.x; // { dg-error ".E0596." "" { target *-*-* } }
        let _ = &mut other.x; // { dg-error ".E0596." "" { target *-*-* } }
    }

    fn foo4(other: &Z) {
        let _ = &mut other.x; // { dg-error ".E0596." "" { target *-*-* } }
    }

}

pub fn with_arg(z: Z, w: &Z) {
    let _ = &mut z.x; // { dg-error ".E0596." "" { target *-*-* } }
    let _ = &mut w.x; // { dg-error ".E0596." "" { target *-*-* } }
}

pub fn with_tuple() {
    let mut y = 0;
    let x = (&y,);
    *x.0 = 1;
// { dg-error ".E0594." "" { target *-*-* } .-1 }
}

