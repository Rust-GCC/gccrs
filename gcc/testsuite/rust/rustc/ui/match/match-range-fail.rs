fn main() {
    match "wow" {
        "bar" ..= "foo" => { }
    };
// { dg-error ".E0029." "" { target *-*-* } .-2 }

    match "wow" {
        10 ..= "what" => ()
    };
// { dg-error ".E0029." "" { target *-*-* } .-2 }

    match "wow" {
        true ..= "what" => {}
    };
// { dg-error ".E0029." "" { target *-*-* } .-2 }

    match 5 {
        'c' ..= 100 => { }
        _ => { }
    };
// { dg-error ".E0308." "" { target *-*-* } .-3 }
}

