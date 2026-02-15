static c: char = '●●';
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {
    let ch: &str = '●●';
// { dg-error "" "" { target *-*-* } .-1 }
}

