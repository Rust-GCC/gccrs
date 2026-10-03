// The original problem in #66340 was that `find_deprecation_generic`
// called `attr.meta().unwrap()` under the assumption that the attribute
// was a well-formed `MetaItem`.

fn main() {
    foo()
}

#[deprecated(note = test)]
// { dg-error "" "" { target *-*-* } .-1 }
fn foo() {}

