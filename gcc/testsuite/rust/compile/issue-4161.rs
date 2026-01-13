
type X = ();

fn main() {
    let () = ::X = (); // { dg-error "expected value, found invalid expression" }
}