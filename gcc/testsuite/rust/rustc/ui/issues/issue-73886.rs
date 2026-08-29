fn main() {
    let _ = &&[0] as &[_];
// { dg-error ".E0605." "" { target *-*-* } .-1 }
    let _ = 7u32 as Option<_>;
// { dg-error ".E0605." "" { target *-*-* } .-1 }
}

