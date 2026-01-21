use std::rc::Rc;
use std::sync::Arc;

struct Foo(Arc<Bar>);

enum Bar {
    A(Rc<Foo>),
    B(Option<Foo>),
}

fn f<T: Send>(_: T) {}

fn main() {
    f(Foo(Arc::new(Bar::B(None))));
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
}

