fn id<T>(x: T) -> T { x }

fn main() {
    let v = vec![
        &id(3)
    ];
// { dg-error ".E0716." "" { target *-*-* } .-2 }

    for &&x in &v {
        println!("{}", x + 3);
    }
}

