// Type ascription doesn't lead to unsoundness

#![feature(type_ascription)]

fn main() {
    let arr = &[1u8, 2, 3];
    let ref x = arr: &[u8]; // { dg-error ".E0308." "" { target *-*-* } }
    let ref mut x = arr: &[u8]; // { dg-error ".E0308." "" { target *-*-* } }
    match arr: &[u8] { // { dg-error ".E0308." "" { target *-*-* } }
        ref x => {}
    }
    let _len = (arr: &[u8]).len(); // { dg-error ".E0308." "" { target *-*-* } }
}

