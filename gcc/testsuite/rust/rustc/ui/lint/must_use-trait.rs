#![deny(unused_must_use)]

#[must_use]
trait Critical {}

trait NotSoCritical {}

trait DecidedlyUnimportant {}

struct Anon;

impl Critical for Anon {}
impl NotSoCritical for Anon {}
impl DecidedlyUnimportant for Anon {}

fn get_critical() -> impl NotSoCritical + Critical + DecidedlyUnimportant {
    Anon {}
}

fn get_boxed_critical() -> Box<dyn Critical> {
    Box::new(Anon {})
}

fn get_nested_boxed_critical() -> Box<Box<dyn Critical>> {
    Box::new(Box::new(Anon {}))
}

fn get_critical_tuple() -> (u32, Box<dyn Critical>, impl Critical, ()) {
    (0, get_boxed_critical(), get_critical(), ())
}

fn main() {
    get_critical(); // { dg-error "" "" { target *-*-* } }
    get_boxed_critical(); // { dg-error "" "" { target *-*-* } }
    get_nested_boxed_critical();
// { dg-error "" "" { target *-*-* } .-1 }
    get_critical_tuple(); // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
}

