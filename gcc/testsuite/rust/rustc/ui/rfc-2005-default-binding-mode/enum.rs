enum Wrapper {
    Wrap(i32),
}

use Wrapper::Wrap;

pub fn main() {
    let Wrap(x) = &Wrap(3);
    *x += 1; // { dg-error ".E0594." "" { target *-*-* } }


    if let Some(x) = &Some(3) {
        *x += 1; // { dg-error ".E0594." "" { target *-*-* } }
    } else {
        panic!();
    }

    while let Some(x) = &Some(3) {
        *x += 1; // { dg-error ".E0594." "" { target *-*-* } }
        break;
    }
}

