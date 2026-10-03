// edition:2018
trait T {
    async fn foo() {} // { dg-error ".E0706." "" { target *-*-* } }
    async fn bar(&self) {} // { dg-error ".E0706." "" { target *-*-* } }
}

fn main() {}

