use baz::zed::bar; // { dg-error ".E0432." "" { target *-*-* } }
// { dg-error ".E0432." "" { target *-*-* } .-1 }

mod baz {}
mod zed {
    pub fn bar() { println!("bar3"); }
}
fn main() {
    bar();
}

