// { dg-output "15\n19\n" }
macro_rules! line {
    () => {{}};
}

extern "C" {
    fn printf(fmt: *const i8, ...);
}

fn print(s: u32) {
    printf("%u\n\0" as *const str as *const i8, s);
}

fn main() -> i32 {
    let line1 = line!();

    print(line1);

    let line2 = line!();

    print(line2);

    0
}
