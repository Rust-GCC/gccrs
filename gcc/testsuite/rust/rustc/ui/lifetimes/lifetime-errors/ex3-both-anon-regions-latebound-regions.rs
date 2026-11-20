fn foo<'a,'b>(x: &mut Vec<&'a u8>, y: &'b u8) {
    x.push(y); // { dg-error ".E0623." "" { target *-*-* } }
}

fn main() { }

