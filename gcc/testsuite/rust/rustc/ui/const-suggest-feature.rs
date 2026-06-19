const WRITE: () = unsafe {
    *std::ptr::null_mut() = 0;
// { dg-error ".E0658." "" { target *-*-* } .-1 }
// { help ".E0658." "" { target *-*-* } .-2 }
};

fn main() {}

