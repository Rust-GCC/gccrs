fn main() {
    let pi = f32::consts::PI; // { dg-error ".E0223." "" { target *-*-* } }
    let bytes = "hello world".as_bytes();
    let string = unsafe {
        str::from_utf8(bytes) // { dg-error ".E0599." "" { target *-*-* } }
    };
}

