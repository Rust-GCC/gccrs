fn cplusplus_mode_exceptionally_unsafe(x: &mut Option<&'static mut isize>) {
    let mut z = (0, 0);
    *x = Some(&mut z.1);
// { dg-error ".E0597." "" { target *-*-* } .-1 }
    panic!("catch me for a dangling pointer!")
}

fn main() {
    cplusplus_mode_exceptionally_unsafe(&mut None);
}

