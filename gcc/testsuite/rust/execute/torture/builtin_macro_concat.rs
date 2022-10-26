// { dg-output "\ntest10btrue2.15\ntest10bfalse2.151\n" }
#[rustc_builtin_macro]
macro_rules! concat {
    () => {{}};
}

extern "C" {
    fn printf(fmt: *const i8, ...);
}

fn print(s: &str) {
    unsafe {
        printf(
            "%s\n" as *const str as *const i8,
            s as *const str as *const i8,
        );
    }
}

fn main() -> i32 {
    let a = concat!();
    let b = concat!("test", 10, 'b', true, 2.15);
    let c = concat!("test", 10, 'b', false, 2.15, 1u64);
    print(a);
    print(b);
    print(c);

    0
}
