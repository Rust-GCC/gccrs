// Test that the use of the non object-safe trait objects
// are gated by `object_safe_for_dispatch` feature gate.

trait NonObjectSafe1: Sized {}

trait NonObjectSafe2 {
    fn static_fn() {}
}

trait NonObjectSafe3 {
    fn foo<T>(&self);
}

trait NonObjectSafe4 {
    fn foo(&self, &Self);
}

fn takes_non_object_safe_ref<T>(obj: &dyn NonObjectSafe1) {
// { dg-error ".E0038." "" { target *-*-* } .-1 }
}

fn return_non_object_safe_ref() -> &'static dyn NonObjectSafe2 {
// { dg-error ".E0038." "" { target *-*-* } .-1 }
    loop {}
}

fn takes_non_object_safe_box(obj: Box<dyn NonObjectSafe3>) {
// { dg-error ".E0038." "" { target *-*-* } .-1 }
}

fn return_non_object_safe_rc() -> std::rc::Rc<dyn NonObjectSafe4> {
// { dg-error ".E0038." "" { target *-*-* } .-1 }
    loop {}
}

trait Trait {}

impl Trait for dyn NonObjectSafe1 {}
// { dg-error ".E0038." "" { target *-*-* } .-1 }

fn main() {}

