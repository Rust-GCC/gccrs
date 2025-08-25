// ignore-compare-mode-chalk

trait Next {
    type Next: Next;
}

struct GetNext<T: Next> { t: T }

impl<T: Next> Next for GetNext<T> {
    type Next = <GetNext<T> as Next>::Next;
// { dg-error ".E0275." "" { target *-*-* } .-1 }
}

fn main() {}

