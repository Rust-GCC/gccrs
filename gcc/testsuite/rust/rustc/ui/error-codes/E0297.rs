fn main() {
    let xs : Vec<Option<i32>> = vec![Some(1), None];

    for Some(x) in xs {}
// { dg-error ".E0005." "" { target *-*-* } .-1 }
}

