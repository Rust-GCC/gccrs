fn f<'a: 'static>(_: &'a i32) {} // { dg-warning "" "" { target *-*-* } }

fn main() {
    let x = 0;
    f(&x); // { dg-error ".E0597." "" { target *-*-* } }
}

