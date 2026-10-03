pub fn test() {
    foo(|_|) // { dg-error "" "" { target *-*-* } }
}

fn main() { }

