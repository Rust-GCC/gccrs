trait B {
    fn foo(mut a: &String) {
        a.push_str("bar"); // { dg-error ".E0596." "" { target *-*-* } }
    }
}

pub fn foo<'a>(mut a: &'a String) {
    a.push_str("foo"); // { dg-error ".E0596." "" { target *-*-* } }
}

struct A {}

impl A {
    pub fn foo(mut a: &String) {
        a.push_str("foo"); // { dg-error ".E0596." "" { target *-*-* } }
    }
}

fn main() {
    foo(&"a".to_string());
    A::foo(&"a".to_string());
}

