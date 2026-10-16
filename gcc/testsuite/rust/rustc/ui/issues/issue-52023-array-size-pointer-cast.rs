fn main() {
    let _ = [0; (&0 as *const i32) as usize]; // { dg-error ".E0080." "" { target *-*-* } }
// { dg-error ".E0080." "" { target *-*-* } .-1 }
}

