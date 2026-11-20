use std::ops::AddAssign;

struct Int(i32);

impl AddAssign for Int {
    fn add_assign(&mut self, _: Int) {
        unimplemented!()
    }
}

fn main() {
    let mut x = Int(1);
    x
// { dg-note "" "" { target *-*-* } .-1 }
    +=
    x;
// { dg-error ".E0505." "" { target *-*-* } .-1 }
// { dg-error ".E0505." "" { target *-*-* } .-2 }

    let y = Int(2);
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
    y   // { dg-error ".E0596." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
    +=
    Int(1);
}

