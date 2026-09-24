fn main () {
    {println!("{:?}", match { let foo = vec![1, 2]; foo.get(1) } { x => x });}
// { dg-error ".E0597." "" { target *-*-* } .-1 }
}

