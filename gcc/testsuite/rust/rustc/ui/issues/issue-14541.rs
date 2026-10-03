struct Vec2 { y: f32 }
struct Vec3 { y: f32, z: f32 }

fn make(v: Vec2) {
    let Vec3 { y: _, z: _ } = v;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

fn main() { }

