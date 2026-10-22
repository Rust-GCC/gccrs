struct S;

impl S {
    #[derive(Debug)] // { dg-error ".E0774." "" { target *-*-* } }
    fn f() {
        file!();
    }
}

trait Tr1 {
    #[derive(Debug)] // { dg-error ".E0774." "" { target *-*-* } }
    fn f();
}

trait Tr2 {
    #[derive(Debug)] // { dg-error ".E0774." "" { target *-*-* } }
    type F;
}

fn main() {}

