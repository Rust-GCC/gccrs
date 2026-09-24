struct Wrap<'a> { w: &'a mut u32 }

fn foo() {
    let mut x = 22;
    let wrapper = Wrap { w: &mut x };
    x += 1; // { dg-error ".E0503." "" { target *-*-* } }
    *wrapper.w += 1;
}

fn main() { }

