static FOO: u32 = 50;

fn main() {
    let _val: &'static [&'static u32] = &[&FOO]; // { dg-error ".E0716." "" { target *-*-* } }
}

