// compile-flags: --error-format json -Zunstable-options
// run-rustfix

// The output for humans should just highlight the whole span without showing
// the suggested replacement, but we also want to test that suggested
// replacement only removes one set of parentheses, rather than naïvely
// stripping away any starting or ending parenthesis characters—hence this
// test of the JSON error format.

#![deny(unused_parens)]
#![allow(unreachable_code)]

fn main() {

    let _b = false;

    if (_b) { // { dg-error "" "" { target *-*-* } }
        println!("hello");
    }

    f();

}

fn f() -> bool {
    let c = false;

    if(c) { // { dg-error "" "" { target *-*-* } }
        println!("next");
    }

    if (c){ // { dg-error "" "" { target *-*-* } }
        println!("prev");
    }

    while (false && true){
        if (c) { // { dg-error "" "" { target *-*-* } }
            println!("norm");
        }

    }

    while(true && false) { // { dg-error "" "" { target *-*-* } }
        for _ in (0 .. 3){ // { dg-error "" "" { target *-*-* } }
            println!("e~")
        }
    }

    for _ in (0 .. 3) { // { dg-error "" "" { target *-*-* } }
        while (true && false) { // { dg-error "" "" { target *-*-* } }
            println!("e~")
        }
    }


    loop {
        if (break { return true }) {
        }
    }
    false
}

