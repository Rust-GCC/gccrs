fn foo(():(), ():()) {}
fn bar(():()) {}

struct S;
impl S {
    fn baz(self, (): ()) { }
    fn generic<T>(self, _: T) { }
}

fn main() {
    let _: Result<(), String> = Ok(); // { dg-error ".E0061." "" { target *-*-* } }
    foo(); // { dg-error ".E0061." "" { target *-*-* } }
    foo(()); // { dg-error ".E0061." "" { target *-*-* } }
    bar(); // { dg-error ".E0061." "" { target *-*-* } }
    S.baz(); // { dg-error ".E0061." "" { target *-*-* } }
    S.generic::<()>(); // { dg-error ".E0061." "" { target *-*-* } }
}

