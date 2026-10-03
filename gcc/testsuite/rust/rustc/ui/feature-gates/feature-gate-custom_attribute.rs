// Check that literals in attributes parse just fine.

#[fake_attr] // { dg-error "" "" { target *-*-* } }
#[fake_attr(100)] // { dg-error "" "" { target *-*-* } }
#[fake_attr(1, 2, 3)] // { dg-error "" "" { target *-*-* } }
#[fake_attr("hello")] // { dg-error "" "" { target *-*-* } }
#[fake_attr(name = "hello")] // { dg-error "" "" { target *-*-* } }
#[fake_attr(1, "hi", key = 12, true, false)] // { dg-error "" "" { target *-*-* } }
#[fake_attr(key = "hello", val = 10)] // { dg-error "" "" { target *-*-* } }
#[fake_attr(key("hello"), val(10))] // { dg-error "" "" { target *-*-* } }
#[fake_attr(enabled = true, disabled = false)] // { dg-error "" "" { target *-*-* } }
#[fake_attr(true)] // { dg-error "" "" { target *-*-* } }
#[fake_attr(pi = 3.14159)] // { dg-error "" "" { target *-*-* } }
#[fake_attr(b"hi")] // { dg-error "" "" { target *-*-* } }
#[fake_doc(r"doc")] // { dg-error "" "" { target *-*-* } }
struct Q {}

fn main() {}

