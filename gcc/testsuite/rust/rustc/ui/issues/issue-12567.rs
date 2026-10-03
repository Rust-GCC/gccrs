fn match_vecs<'a, T>(l1: &'a [T], l2: &'a [T]) {
    match (l1, l2) {
// { dg-error ".E0508." "" { target *-*-* } .-1 }
// { dg-error ".E0508." "" { target *-*-* } .-2 }
        (&[], &[]) => println!("both empty"),
        (&[], &[hd, ..]) | (&[hd, ..], &[])
            => println!("one empty"),
        (&[hd1, ..], &[hd2, ..])
            => println!("both nonempty"),
    }
}

fn main() {}

