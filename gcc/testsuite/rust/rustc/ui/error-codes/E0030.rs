fn main() {
    match 5u32 {
        1000 ..= 5 => {}
// { dg-error ".E0030." "" { target *-*-* } .-1 }
// { dg-error ".E0030." "" { target *-*-* } .-2 }
    }
}

