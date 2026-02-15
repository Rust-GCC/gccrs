mod foo { pub fn bar() {} }

fn main() {
    match () {
        foo::bar => {}
// { dg-error ".E0532." "" { target *-*-* } .-1 }
    }
}

