fn main() {
    let error = error; // { dg-error ".E0425." "" { target *-*-* } }

    // These used to cause errors.
    0 as f32;
    0.0 as u32;
}

