fn call_it<F>(f: F) where F: Fn() { f(); }

struct A;

impl A {
    fn gen(&self) {}
    fn gen_mut(&mut self) {}
}

fn main() {
    let mut x = A;
    call_it(|| {
        call_it(|| x.gen());
        call_it(|| x.gen_mut());
// { dg-error ".E0596." "" { target *-*-* } .-1 }
// { dg-error ".E0596." "" { target *-*-* } .-2 }
    });
}

