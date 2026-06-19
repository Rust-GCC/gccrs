// ignore-tidy-linelength

fn func((1, (Some(1), 2..=3)): (isize, (Option<isize>, isize))) { }
// { dg-error ".E0005." "" { target *-*-* } .-1 }

fn main() {
    let (1, (Some(1), 2..=3)) = (1, (None, 2));
// { dg-error ".E0005." "" { target *-*-* } .-1 }
}

