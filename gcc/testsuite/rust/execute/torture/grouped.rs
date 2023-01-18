// { dg-output "12\n" }
extern "C" {
    fn printf(s: *const i8, ...);
}

fn main() {
    let (a) = 12;
    unsafe { printf("%d\n" as *const str as *const i8, a) }
}
