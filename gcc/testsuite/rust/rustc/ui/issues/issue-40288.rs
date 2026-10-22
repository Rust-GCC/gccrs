fn save_ref<'a>(refr: &'a i32, to: &mut [&'a i32]) {
    for val in &mut *to {
        *val = refr;
    }
}

fn main() {
    let ref init = 0i32;
    let ref mut refr = 1i32;

    let mut out = [init];

    save_ref(&*refr, &mut out);

    // This shouldn't be allowed as `refr` is borrowed
    *refr = 3; // { dg-error ".E0506." "" { target *-*-* } }

    // Prints 3?!
    println!("{:?}", out[0]);
}

