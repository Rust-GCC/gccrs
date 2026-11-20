struct Inches(i32);

fn main() {
    Inches as f32;
// { dg-error ".E0606." "" { target *-*-* } .-1 }
}

