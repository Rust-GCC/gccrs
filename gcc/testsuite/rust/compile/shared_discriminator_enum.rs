// from issue 3351: https://github.com/Rust-GCC/gccrs/issues/3351
enum Foo { // { dg-error "discriminant value 1 assigned more than once" }
    Bar = 1, 
    Qaz = 1, 
}

fn main() {}
