fn main() {
    match "world" { // { dg-error ".E0004." "" { target *-*-* } }
        "hello" => {}
    }

    match "world" { // { dg-error ".E0004." "" { target *-*-* } }
        ref _x if false => {}
        "hello" => {}
    }
}

