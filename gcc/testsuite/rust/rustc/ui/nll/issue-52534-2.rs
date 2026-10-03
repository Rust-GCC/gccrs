fn foo(x: &u32) -> &u32 {
    let y;

    {
        let x = 32;
        y = &x
// { dg-error ".E0597." "" { target *-*-* } .-1 }
    }

    println!("{}", y);
    x
}

fn main() { }

