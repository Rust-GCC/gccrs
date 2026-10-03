fn main() {
    for _ in [0..1] {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    for _ in [0..=1] {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    for _ in [0..] {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    for _ in [..1] {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    for _ in [..=1] {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    let start = 0;
    let end = 0;
    for _ in [start..end] {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    let array_of_range = [start..end];
    for _ in array_of_range {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    for _ in [0..1, 2..3] {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    for _ in [0..=1] {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

