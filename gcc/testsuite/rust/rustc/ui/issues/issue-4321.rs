fn main() {
    let tup = (true, true);
    println!("foo {:}", match tup { // { dg-error ".E0004." "" { target *-*-* } }
        (false, false) => "foo",
        (false, true) => "bar",
        (true, true) => "baz"
    });
}

