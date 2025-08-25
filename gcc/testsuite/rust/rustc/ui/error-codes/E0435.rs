fn main () {
    let foo = 42u32;
    let _: [u8; foo]; // { dg-error ".E0435." "" { target *-*-* } }
}

