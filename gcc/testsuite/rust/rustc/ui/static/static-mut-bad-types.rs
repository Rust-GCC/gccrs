static mut a: isize = 3;

fn main() {
    unsafe {
        a = true; // { dg-error ".E0308." "" { target *-*-* } }
    }
}

