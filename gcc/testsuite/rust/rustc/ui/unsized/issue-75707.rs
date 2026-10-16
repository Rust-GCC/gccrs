pub trait Callback {
    fn cb();
}

pub trait Processing {
    type Call: Callback;
}

fn f<P: Processing + ?Sized>() {
    P::Call::cb();
}

fn main() {
    struct MyCall;
    f::<dyn Processing<Call = MyCall>>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

