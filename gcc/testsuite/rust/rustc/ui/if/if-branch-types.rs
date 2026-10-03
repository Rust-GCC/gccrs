fn main() {
    let x = if true { 10i32 } else { 10u32 };
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

