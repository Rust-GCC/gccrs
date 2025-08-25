extern {
    fn printf(c: *const i8, ...);
}

fn main() {
    unsafe {
        printf(::std::ptr::null(), 0f32);
// { dg-error ".E0617." "" { target *-*-* } .-1 }
// { help ".E0617." "" { target *-*-* } .-2 }
        printf(::std::ptr::null(), 0i8);
// { dg-error ".E0617." "" { target *-*-* } .-1 }
// { help ".E0617." "" { target *-*-* } .-2 }
        printf(::std::ptr::null(), 0i16);
// { dg-error ".E0617." "" { target *-*-* } .-1 }
// { help ".E0617." "" { target *-*-* } .-2 }
        printf(::std::ptr::null(), 0u8);
// { dg-error ".E0617." "" { target *-*-* } .-1 }
// { help ".E0617." "" { target *-*-* } .-2 }
        printf(::std::ptr::null(), 0u16);
// { dg-error ".E0617." "" { target *-*-* } .-1 }
// { help ".E0617." "" { target *-*-* } .-2 }
        printf(::std::ptr::null(), printf);
// { dg-error ".E0617." "" { target *-*-* } .-1 }
// { help ".E0617." "" { target *-*-* } .-2 }
    }
}

