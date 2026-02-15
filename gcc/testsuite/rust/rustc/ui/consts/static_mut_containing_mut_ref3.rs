static mut FOO: (u8, u8) = (42, 43);

static mut BAR: () = unsafe { FOO.0 = 99; };
// { dg-error ".E0080." "" { target *-*-* } .-1 }

fn main() {}

