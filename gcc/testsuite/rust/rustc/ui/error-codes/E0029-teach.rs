// compile-flags: -Z teach

fn main() {
    let s = "hoho";

    match s {
        "hello" ..= "world" => {}
// { dg-error ".E0029." "" { target *-*-* } .-1 }
        _ => {}
    }
}

