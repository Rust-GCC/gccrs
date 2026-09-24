struct Test;

impl Test {
    fn bar(&self, x: &u32) -> &u32 {
        let x = 22;
        &x
// { dg-error ".E0515." "" { target *-*-* } .-1 }
    }
}

fn foo(x: &u32) -> &u32 {
    let x = 22;
    &x
// { dg-error ".E0515." "" { target *-*-* } .-1 }
}

fn baz(x: &u32) -> &&u32 {
    let x = 22;
    &&x
// { dg-error ".E0515." "" { target *-*-* } .-1 }
// { dg-error ".E0515." "" { target *-*-* } .-2 }
}

fn foobazbar<'a>(x: u32, y: &'a u32) -> &'a u32 {
    let x = 22;
    &x
// { dg-error ".E0515." "" { target *-*-* } .-1 }
}

fn foobar<'a>(x: &'a u32) -> &'a u32 {
    let x = 22;
    &x
// { dg-error ".E0515." "" { target *-*-* } .-1 }
}

fn foobaz<'a, 'b>(x: &'a u32, y: &'b u32) -> &'a u32 {
    let x = 22;
    &x
// { dg-error ".E0515." "" { target *-*-* } .-1 }
}

fn foobarbaz<'a, 'b>(x: &'a u32, y: &'b u32, z: &'a u32) -> &'a u32 {
    let x = 22;
    &x
// { dg-error ".E0515." "" { target *-*-* } .-1 }
}

fn main() { }

