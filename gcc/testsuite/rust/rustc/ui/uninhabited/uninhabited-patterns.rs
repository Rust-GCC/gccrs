#![feature(box_patterns)]
#![feature(box_syntax)]
#![feature(never_type)]
#![feature(exhaustive_patterns)]

#![deny(unreachable_patterns)]

mod foo {
    pub struct SecretlyEmpty {
        _priv: !,
    }
}

struct NotSoSecretlyEmpty {
    _priv: !,
}

fn foo() -> Option<NotSoSecretlyEmpty> {
    None
}

fn main() {
    let x: &[!] = &[];

    match x {
        &[]   => (),
        &[..] => (),    // { dg-error "" "" { target *-*-* } }
    };

    let x: Result<Box<NotSoSecretlyEmpty>, &[Result<!, !>]> = Err(&[]);
    match x {
        Ok(box _) => (),    // { dg-error "" "" { target *-*-* } }
        Err(&[]) => (),
        Err(&[..]) => (),   // { dg-error "" "" { target *-*-* } }
    }

    let x: Result<foo::SecretlyEmpty, Result<NotSoSecretlyEmpty, u32>> = Err(Err(123));
    match x {
        Ok(_y) => (),
        Err(Err(_y)) => (),
        Err(Ok(_y)) => (),  // { dg-error "" "" { target *-*-* } }
    }

    while let Some(_y) = foo() {
// { dg-error "" "" { target *-*-* } .-1 }
    }
}

