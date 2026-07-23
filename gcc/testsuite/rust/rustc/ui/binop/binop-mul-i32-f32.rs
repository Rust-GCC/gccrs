fn foo(x: i32, y: f32) -> f32 {
    x * y // { dg-error ".E0277." "" { target *-*-* } }
}

fn main() {}

