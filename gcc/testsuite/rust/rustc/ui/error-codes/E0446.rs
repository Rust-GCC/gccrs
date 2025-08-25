mod foo {
    struct Bar(u32);

    pub fn bar() -> Bar { // { dg-error ".E0446." "" { target *-*-* } }
        Bar(0)
    }
}

fn main() {}

