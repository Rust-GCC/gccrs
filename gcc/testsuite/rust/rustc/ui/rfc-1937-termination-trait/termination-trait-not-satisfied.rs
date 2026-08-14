struct ReturnType {}

fn main() -> ReturnType { // { dg-error ".E0277." "" { target *-*-* } }
    ReturnType {}
}

