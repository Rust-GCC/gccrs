fn foo(_s: i16) { }

fn bar(_s: u32) { }

fn main() {
    foo(1*(1 as isize));
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }

    bar(1*(1 as usize));
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

