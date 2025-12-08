fn main() {
    // no complaints about the tuple not matching the expected type
    let x: (usize, usize, usize) = (3, .=.);
// { dg-error "" "" { target *-*-* } .-1 }
    // verify that the parser recovers:
    let y: usize = ""; // { dg-error ".E0308." "" { target *-*-* } }
    // no complaints about the type
    foo(x);
}

fn foo(_: (usize, usize, usize)) {}

