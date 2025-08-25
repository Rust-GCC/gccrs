#[derive(Copy, Clone)]
struct S;

impl S {
    fn mutate(&mut self) {
    }
}

fn func(arg: S) {
    arg.mutate(); // { dg-error ".E0596." "" { target *-*-* } }
}

impl S {
    fn method(&self, arg: S) {
        arg.mutate(); // { dg-error ".E0596." "" { target *-*-* } }
    }
}

trait T {
    fn default(&self, arg: S) {
        arg.mutate(); // { dg-error ".E0596." "" { target *-*-* } }
    }
}

impl T for S {}

fn main() {
    let s = S;
    func(s);
    s.method(s);
    s.default(s);
    (|arg: S| { arg.mutate() })(s);
// { dg-error ".E0596." "" { target *-*-* } .-1 }
}

