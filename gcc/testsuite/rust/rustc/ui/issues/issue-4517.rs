fn bar(int_param: usize) {}

fn main() {
    let foo: [u8; 4] = [1; 4];
    bar(foo);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

