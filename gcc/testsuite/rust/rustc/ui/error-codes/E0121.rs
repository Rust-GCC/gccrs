fn foo() -> _ { 5 } // { dg-error ".E0121." "" { target *-*-* } }

static BAR: _ = "test"; // { dg-error ".E0121." "" { target *-*-* } }

fn main() {
}

