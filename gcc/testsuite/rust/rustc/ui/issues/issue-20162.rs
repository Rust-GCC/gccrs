struct X { x: i32 }

fn main() {
    let mut b: Vec<X> = vec![];
    b.sort();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

