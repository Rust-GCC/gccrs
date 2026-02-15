use zed::bar;
use zed::baz; // { dg-error ".E0432." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
// { help "" "" { target *-*-* } .-3 }
// { suggestion "" "" { target *-*-* } .-4 }


mod zed {
    pub fn bar() { println!("bar"); }
    use foo; // { dg-error ".E0432." "" { target *-*-* } }
// { dg-error ".E0432." "" { target *-*-* } .-1 }
}

fn main() {
    zed::foo(); // { dg-error ".E0603." "" { target *-*-* } }
    bar();
}

