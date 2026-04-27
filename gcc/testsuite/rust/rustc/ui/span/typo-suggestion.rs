fn main() {
    let foo = 1;

    // `foo` shouldn't be suggested, it is too dissimilar from `bar`.
    println!("Hello {}", bar); // { dg-error ".E0425." "" { target *-*-* } }

    // But this is close enough.
    println!("Hello {}", fob); // { dg-error ".E0425." "" { target *-*-* } }
}

