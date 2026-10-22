// Various checks that deprecation attributes are used correctly

mod bogus_attribute_types_1 {
    #[deprecated(since = "a", note = "a", reason)] // { dg-error ".E0541." "" { target *-*-* } }
    fn f1() { }

    #[deprecated(since = "a", note)] // { dg-error ".E0551." "" { target *-*-* } }
    fn f2() { }

    #[deprecated(since, note = "a")] // { dg-error ".E0551." "" { target *-*-* } }
    fn f3() { }

    #[deprecated(since = "a", note(b))] // { dg-error ".E0551." "" { target *-*-* } }
    fn f5() { }

    #[deprecated(since(b), note = "a")] // { dg-error ".E0551." "" { target *-*-* } }
    fn f6() { }

    #[deprecated(note = b"test")] // { dg-error ".E0565." "" { target *-*-* } }
    fn f7() { }

    #[deprecated("test")] // { dg-error ".E0565." "" { target *-*-* } }
    fn f8() { }
}

#[deprecated(since = "a", note = "b")]
#[deprecated(since = "a", note = "b")] // { dg-error ".E0550." "" { target *-*-* } }
fn multiple1() { }

#[deprecated(since = "a", since = "b", note = "c")] // { dg-error ".E0538." "" { target *-*-* } }
fn f1() { }

struct X;

#[deprecated = "hello"] // { dg-error "" "" { target *-*-* } }
impl Default for X {
    fn default() -> Self {
        X
    }
}

fn main() { }

