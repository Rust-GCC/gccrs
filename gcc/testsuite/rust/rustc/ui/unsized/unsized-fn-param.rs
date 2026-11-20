use std::convert::AsRef;
use std::path::Path;

fn foo11(_bar: &dyn AsRef<Path>, _baz: &str) {}
fn foo12(_bar: &str, _baz: &dyn AsRef<Path>) {}

fn foo21(_bar: &dyn AsRef<str>, _baz: &str) {}
fn foo22(_bar: &str, _baz: &dyn AsRef<str>) {}

fn main() {
    foo11("bar", &"baz"); // { dg-error ".E0277." "" { target *-*-* } }
    foo11(&"bar", &"baz");
    foo12(&"bar", "baz"); // { dg-error ".E0277." "" { target *-*-* } }
    foo12(&"bar", &"baz");

    foo21("bar", &"baz"); // { dg-error ".E0277." "" { target *-*-* } }
    foo21(&"bar", &"baz");
    foo22(&"bar", "baz"); // { dg-error ".E0277." "" { target *-*-* } }
    foo22(&"bar", &"baz");
}

