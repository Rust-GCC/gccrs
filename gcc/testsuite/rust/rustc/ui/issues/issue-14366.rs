fn main() {
    let _x = "test" as &dyn (::std::any::Any);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

