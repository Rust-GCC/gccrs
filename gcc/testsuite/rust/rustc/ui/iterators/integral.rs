fn main() {
    for _ in 42 {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    for _ in 42 as u8 {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    for _ in 42 as i8 {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    for _ in 42 as u16 {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    for _ in 42 as i16 {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    for _ in 42 as u32 {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    for _ in 42 as i32 {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    for _ in 42 as u64 {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    for _ in 42 as i64 {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    for _ in 42 as usize {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    for _ in 42 as isize {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    for _ in 42.0 {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

