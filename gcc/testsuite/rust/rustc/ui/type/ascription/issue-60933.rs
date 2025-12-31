// run-rustfix
fn main() {
    let _: usize = std::mem:size_of::<u32>();
// { dg-error "" "" { target *-*-* } .-1 }
}

