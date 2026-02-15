fn assert_sizeof() -> ! {
    unsafe {
        ::std::mem::transmute::<f64, [u8; 8]>(panic!())
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    }
}

fn main() { }

