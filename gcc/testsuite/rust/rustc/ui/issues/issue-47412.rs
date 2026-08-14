#[derive(Copy, Clone)]
enum Void {}

// Tests that we detect unsafe places (specifically, union fields and
// raw pointer dereferences), even when they're matched on while having
// an uninhabited type (equivalent to `std::intrinsics::unreachable()`).

fn union_field() {
    union Union { unit: (), void: Void }
    let u = Union { unit: () };
    match u.void {}
// { dg-error ".E0133." "" { target *-*-* } .-1 }
}

fn raw_ptr_deref() {
    let ptr = std::ptr::null::<Void>();
    match *ptr {}
// { dg-error ".E0133." "" { target *-*-* } .-1 }
}

fn main() {}

