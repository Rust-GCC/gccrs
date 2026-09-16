fn main() {
    let mut my_stuff = std::collections::HashMap::new();
    my_stuff.insert(0, 42);

    let mut it = my_stuff.iter();
    my_stuff.insert(1, 43); // { dg-error ".E0502." "" { target *-*-* } }
    it;
}

