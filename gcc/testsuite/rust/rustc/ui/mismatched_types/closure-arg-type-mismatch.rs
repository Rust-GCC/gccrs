fn main() {
    let a = [(1u32, 2u32)];
    a.iter().map(|_: (u32, u32)| 45); // { dg-error ".E0631." "" { target *-*-* } }
    a.iter().map(|_: &(u16, u16)| 45); // { dg-error ".E0631." "" { target *-*-* } }
    a.iter().map(|_: (u16, u16)| 45); // { dg-error ".E0631." "" { target *-*-* } }
}

fn baz<F: Fn(*mut &u32)>(_: F) {}
fn _test<'a>(f: fn(*mut &'a u32)) {
    baz(f);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
}

