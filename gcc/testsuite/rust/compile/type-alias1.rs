type TypeAlias = (i32, u32);

fn main() {
    let a: TypeAlias;
    a = (123, 456f32); // { dg-error "expected .u32. got .f32." }
}
