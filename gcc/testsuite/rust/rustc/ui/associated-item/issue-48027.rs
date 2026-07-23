trait Bar {
    const X: usize;
    fn return_n(&self) -> [u8; Bar::X]; // { dg-error ".E0283." "" { target *-*-* } }
}

impl dyn Bar {} // { dg-error ".E0038." "" { target *-*-* } }

fn main() {}

