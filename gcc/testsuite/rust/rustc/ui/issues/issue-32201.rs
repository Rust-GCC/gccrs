extern {
    fn foo(a: i32, ...);
}

fn bar(_: *const u8) {}

fn main() {
    unsafe {
        foo(0, bar);
// { dg-error ".E0617." "" { target *-*-* } .-1 }
// { help ".E0617." "" { target *-*-* } .-2 }
    }
}

