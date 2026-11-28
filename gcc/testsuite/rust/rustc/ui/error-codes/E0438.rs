trait Bar {}

impl Bar for i32 {
    const BAR: bool = true; // { dg-error ".E0438." "" { target *-*-* } }
}

fn main () {
}

