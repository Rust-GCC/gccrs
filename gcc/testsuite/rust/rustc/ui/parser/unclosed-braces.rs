struct S {
    x: [usize; 3],
}

fn foo() {
    {
        {
            println!("hi");
        }
    }
}

fn main() {
// { dg-note "" "" { target *-*-* } .-1 }
    {
        {
// { dg-note "" "" { target *-*-* } .-1 }
            foo();
    }
// { dg-note "" "" { target *-*-* } .-1 }
}
// { dg-error "" "" { target *-*-* } }

