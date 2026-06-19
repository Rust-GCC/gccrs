mod a {
}

trait A {
}

impl A for a { // { dg-error ".E0573." "" { target *-*-* } }
}

fn main() {
}

