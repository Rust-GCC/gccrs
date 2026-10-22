struct Binder(i32, i32, i32);

fn main() {
    let x = Binder(1, 2, 3);
    match x {
        Binder(_a, _x @ ..) => {}
        _ => {}
    }
}
// { dg-error ".E0023." "" { target *-*-* } .-4 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error ".E0023." "" { target *-*-* } .-3 }

