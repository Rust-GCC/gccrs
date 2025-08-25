fn main() {
    match 0 {
        aaa::bbb(_) => ()
// { dg-error ".E0433." "" { target *-*-* } .-1 }
    };
}

