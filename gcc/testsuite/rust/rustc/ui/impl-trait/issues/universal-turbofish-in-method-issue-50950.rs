use std::any::Any;
pub struct EventHandler {
}

impl EventHandler
{
    pub fn handle_event<T: Any>(&mut self, _efunc: impl FnMut(T)) {}
}

struct TestEvent(i32);

fn main() {
    let mut evt = EventHandler {};
    evt.handle_event::<TestEvent, fn(TestEvent)>(|_evt| {
// { dg-error ".E0632." "" { target *-*-* } .-1 }
    });
}

