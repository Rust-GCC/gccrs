// edition:2018

// Exercise the unused_mut attribute in some positive and negative cases

#![warn(unused_mut)]
#![feature(async_closure, raw_ref_op)]

async fn baz_async(
    mut a: i32,
// { dg-warning "" "" { target *-*-* } .-1 }
    #[allow(unused_mut)] mut b: i32,
) {}
fn baz(
    mut a: i32,
// { dg-warning "" "" { target *-*-* } .-1 }
    #[allow(unused_mut)] mut b: i32,
    #[allow(unused_mut)] (mut c, d): (i32, i32)
) {}

struct RefStruct {}
impl RefStruct {
    async fn baz_async(
        mut a: i32,
// { dg-warning "" "" { target *-*-* } .-1 }
        #[allow(unused_mut)] mut b: i32,
    ) {}
    fn baz(
        &self,
        mut a: i32,
// { dg-warning "" "" { target *-*-* } .-1 }
        #[allow(unused_mut)] mut b: i32,
        #[allow(unused_mut)] (mut c, d): (i32, i32)
    ) {}
}

trait RefTrait {
    fn baz(
        &self,
        mut a: i32,
// { dg-warning "" "" { target *-*-* } .-1 }
        #[allow(unused_mut)] mut b: i32,
        #[allow(unused_mut)] (mut c, d): (i32, i32)
    ) {}
}
impl RefTrait for () {
    fn baz(
        &self,
        mut a: i32,
// { dg-warning "" "" { target *-*-* } .-1 }
        #[allow(unused_mut)] mut b: i32,
        #[allow(unused_mut)] (mut c, d): (i32, i32)
    ) {}
}

fn main() {
    let _ = async move |
        mut a: i32,
// { dg-warning "" "" { target *-*-* } .-1 }
        #[allow(unused_mut)] mut b: i32,
    | {};
    let _ = |
        mut a: i32,
// { dg-warning "" "" { target *-*-* } .-1 }
        #[allow(unused_mut)] mut b: i32,
        #[allow(unused_mut)] (mut c, d): (i32, i32)
    | {};

    // negative cases
    let mut a = 3; // { dg-warning "" "" { target *-*-* } }

    let mut a = 2; // { dg-warning "" "" { target *-*-* } }

    let mut b = 3; // { dg-warning "" "" { target *-*-* } }

    let mut a = vec![3]; // { dg-warning "" "" { target *-*-* } }

    let (mut a, b) = (1, 2); // { dg-warning "" "" { target *-*-* } }

    let mut a; // { dg-warning "" "" { target *-*-* } }

    a = 3;

    let mut b; // { dg-warning "" "" { target *-*-* } }

    if true {
        b = 3;
    } else {
        b = 4;
    }

    match 30 {
        mut x => {} // { dg-warning "" "" { target *-*-* } }

    }

    match (30, 2) {
        // FIXME: Here's a false positive,
        // shouldn't be removed `mut` not to be bound with a different way.
        (mut x, 1) | // { dg-warning "" "" { target *-*-* } }

        (mut x, 2) |
        (mut x, 3) => {
        }
        _ => {}
    }

    let x = |mut y: isize| 10; // { dg-warning "" "" { target *-*-* } }

    fn what(mut foo: isize) {} // { dg-warning "" "" { target *-*-* } }


    let mut a = &mut 5; // { dg-warning "" "" { target *-*-* } }

    *a = 4;

    let mut a = 5;
    let mut b = (&mut a,); // { dg-warning "" "" { target *-*-* } }
    *b.0 = 4;

    let mut x = &mut 1; // { dg-warning "" "" { target *-*-* } }

    let mut f = || {
      *x += 1;
    };
    f();

    fn mut_ref_arg(mut arg : &mut [u8]) -> &mut [u8] {
// { dg-warning "" "" { target *-*-* } .-1 }

    }

    let mut v : &mut Vec<()> = &mut vec![]; // { dg-warning "" "" { target *-*-* } }

    v.push(());

    // positive cases
    let mut a = 2;
    a = 3;
    let mut a = Vec::new();
    a.push(3);
    let mut a = Vec::new();
    callback(|| {
        a.push(3);
    });
    let mut a = Vec::new();
    callback(|| {
        callback(|| {
            a.push(3);
        });
    });
    let (mut a, b) = (1, 2);
    a = 34;

    match 30 {
        mut x => {
            x = 21;
        }
    }

    match (30, 2) {
      (mut x, 1) |
      (mut x, 2) |
      (mut x, 3) => {
        x = 21
      }
      _ => {}
    }

    // Attribute should be respected on match arms
    match 0 {
        #[allow(unused_mut)]
        mut x => {
            let mut y = 1;
        },
    }

    let x = |mut y: isize| y = 32;
    fn nothing(mut foo: isize) { foo = 37; }

    // leading underscore should avoid the warning, just like the
    // unused variable lint.
    let mut _allowed = 1;

    let mut raw_address_of_mut = 1; // OK
    let mut_ptr = &raw mut raw_address_of_mut;

    let mut raw_address_of_const = 1; // { dg-warning "" "" { target *-*-* } }
    let const_ptr = &raw const raw_address_of_const;
}

fn callback<F>(f: F) where F: FnOnce() {}

// make sure the lint attribute can be turned off
#[allow(unused_mut)]
fn foo(mut a: isize) {
    let mut a = 3;
    let mut b = vec![2];
}

// make sure the lint attribute can be turned off on let statements
#[deny(unused_mut)]
fn bar() {
    #[allow(unused_mut)]
    let mut a = 3;
    let mut b = vec![2]; // { dg-error "" "" { target *-*-* } }

}

