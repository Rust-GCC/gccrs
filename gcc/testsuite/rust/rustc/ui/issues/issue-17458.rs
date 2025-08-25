static X: usize = unsafe { core::ptr::null::<usize>() as usize };
// { dg-error ".E0658." "" { target *-*-* } .-1 }

fn main() {
    assert_eq!(X, 0);
}

