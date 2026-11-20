trait Trait {}

// Make sure casts between thin-pointer <-> fat pointer obey RFC401
fn main() {
    let a: &[i32] = &[1, 2, 3];
    let b: Box<[i32]> = Box::new([1, 2, 3]);
    let p = a as *const [i32];
    let q = a.as_ptr();

    a as usize; // { dg-error ".E0606." "" { target *-*-* } }
    a as isize; // { dg-error ".E0606." "" { target *-*-* } }
    a as i16; // { dg-error ".E0606." "" { target *-*-* } }
    a as u32; // { dg-error ".E0606." "" { target *-*-* } }
    b as usize; // { dg-error ".E0605." "" { target *-*-* } }
    p as usize;
// { dg-error ".E0606." "" { target *-*-* } .-1 }

    // #22955
    q as *const [i32]; // { dg-error ".E0607." "" { target *-*-* } }

    // #21397
    let t: *mut (dyn Trait + 'static) = 0 as *mut _; // { dg-error ".E0606." "" { target *-*-* } }
    let mut fail: *const str = 0 as *const str; // { dg-error ".E0606." "" { target *-*-* } }
}

