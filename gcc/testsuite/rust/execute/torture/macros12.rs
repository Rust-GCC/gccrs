// { dg-output "0\n" }
extern "C" {
    fn printf(s: *const i8, ...);
}

fn print_int(value: i32) {
    let s = "%d\n\0" as *const str as *const i8;
    unsafe {
        printf(s, value);
    }
}

macro_rules! add_exprs {
    ($($e:expr)?) => (0 $(+ $e)?)
}

fn main() -> i32 {
    // 0
    print_int(add_exprs!());

    0
}
