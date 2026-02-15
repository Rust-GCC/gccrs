const TUP: (usize,) = (42,);

fn main() {
    let a: [isize; TUP.1];
// { dg-error ".E0609." "" { target *-*-* } .-1 }
}

