// Tests that we forbid coercion from `[T; n]` to `&[T]`

fn main() {
    let _: &[i32] = [0];
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

