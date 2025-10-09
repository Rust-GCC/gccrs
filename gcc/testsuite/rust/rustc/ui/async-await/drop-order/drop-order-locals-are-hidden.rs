// edition:2018

async fn foobar_async(x: u32, (a, _, _c): (u32, u32, u32), _: u32, _y: u32) {
    assert_eq!(__arg1, (1, 2, 3)); // { dg-error ".E0425." "" { target *-*-* } }
    assert_eq!(__arg2, 4); // { dg-error ".E0425." "" { target *-*-* } }
}

async fn baz_async(ref mut x: u32, ref y: u32) {
    assert_eq!(__arg0, 1); // { dg-error ".E0425." "" { target *-*-* } }
    assert_eq!(__arg1, 2); // { dg-error ".E0425." "" { target *-*-* } }
}

fn main() {}

