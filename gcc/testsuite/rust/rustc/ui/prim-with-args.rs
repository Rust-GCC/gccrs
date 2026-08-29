fn main() {

let x: isize<isize>; // { dg-error ".E0109." "" { target *-*-* } }
let x: i8<isize>; // { dg-error ".E0109." "" { target *-*-* } }
let x: i16<isize>; // { dg-error ".E0109." "" { target *-*-* } }
let x: i32<isize>; // { dg-error ".E0109." "" { target *-*-* } }
let x: i64<isize>; // { dg-error ".E0109." "" { target *-*-* } }
let x: usize<isize>; // { dg-error ".E0109." "" { target *-*-* } }
let x: u8<isize>; // { dg-error ".E0109." "" { target *-*-* } }
let x: u16<isize>; // { dg-error ".E0109." "" { target *-*-* } }
let x: u32<isize>; // { dg-error ".E0109." "" { target *-*-* } }
let x: u64<isize>; // { dg-error ".E0109." "" { target *-*-* } }
let x: char<isize>; // { dg-error ".E0109." "" { target *-*-* } }

let x: isize<'static>; // { dg-error ".E0109." "" { target *-*-* } }
let x: i8<'static>; // { dg-error ".E0109." "" { target *-*-* } }
let x: i16<'static>; // { dg-error ".E0109." "" { target *-*-* } }
let x: i32<'static>; // { dg-error ".E0109." "" { target *-*-* } }
let x: i64<'static>; // { dg-error ".E0109." "" { target *-*-* } }
let x: usize<'static>; // { dg-error ".E0109." "" { target *-*-* } }
let x: u8<'static>; // { dg-error ".E0109." "" { target *-*-* } }
let x: u16<'static>; // { dg-error ".E0109." "" { target *-*-* } }
let x: u32<'static>; // { dg-error ".E0109." "" { target *-*-* } }
let x: u64<'static>; // { dg-error ".E0109." "" { target *-*-* } }
let x: char<'static>; // { dg-error ".E0109." "" { target *-*-* } }

}

