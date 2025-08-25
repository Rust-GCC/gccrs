fn f(f: dyn FnOnce()) {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }

fn main() {
}

