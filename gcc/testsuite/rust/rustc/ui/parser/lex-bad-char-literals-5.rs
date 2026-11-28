static c: char = '\x10\x10';
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {
    let ch: &str = '\x10\x10';
// { dg-error "" "" { target *-*-* } .-1 }
}

