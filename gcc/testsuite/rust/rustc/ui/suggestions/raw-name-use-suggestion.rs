mod foo {
    pub fn r#let() {}
    pub fn break() {} // { dg-error "" "" { target *-*-* } }
}

fn main() {
    foo::let(); // { dg-error "" "" { target *-*-* } }
    r#break(); // { dg-error ".E0425." "" { target *-*-* } }
}

