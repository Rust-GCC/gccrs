// Regression test for issue #38899

pub struct Block<'a> {
    current: &'a u8,
    unrelated: &'a u8,
}

fn bump<'a>(mut block: &mut Block<'a>) {
    let x = &mut block;
    println!("{}", x.current);
    let p: &'a u8 = &*block.current;
// { dg-error ".E0502." "" { target *-*-* } .-1 }
    drop(x);
    drop(p);
}

fn main() {}

