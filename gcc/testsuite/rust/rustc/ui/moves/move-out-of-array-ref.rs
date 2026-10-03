// Ensure that we cannot move out of a reference to a fixed-size array

struct D { _x: u8 }

impl Drop for D { fn drop(&mut self) { } }

fn move_elem(a: &[D; 4]) -> D {
    let [_, e, _, _] = *a;              // { dg-error ".E0508." "" { target *-*-* } }
    e
}

fn move_subarr(a: &[D; 4]) -> [D; 2] {
    let [_, s @ .. , _] = *a;           // { dg-error ".E0508." "" { target *-*-* } }
    s
}

fn move_elem_mut(a: &mut [D; 4]) -> D {
    let [_, e, _, _] = *a;              // { dg-error ".E0508." "" { target *-*-* } }
    e
}

fn move_subarr_mut(a: &mut [D; 4]) -> [D; 2] {
    let [_, s @ .. , _] = *a;           // { dg-error ".E0508." "" { target *-*-* } }
    s
}

fn main() {
    fn d() -> D { D { _x: 0 } }

    move_elem(&[d(), d(), d(), d()]);
    move_subarr(&[d(), d(), d(), d()]);
    move_elem_mut(&mut [d(), d(), d(), d()]);
    move_subarr_mut(&mut [d(), d(), d(), d()]);
}

