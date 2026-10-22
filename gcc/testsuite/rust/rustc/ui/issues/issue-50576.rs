fn main() {
    |bool: [u8; break 'L]| 0;
// { dg-error ".E0268." "" { target *-*-* } .-1 }
// { dg-error ".E0268." "" { target *-*-* } .-2 }
    Vec::<[u8; break]>::new(); // { dg-error ".E0268." "" { target *-*-* } }
}

