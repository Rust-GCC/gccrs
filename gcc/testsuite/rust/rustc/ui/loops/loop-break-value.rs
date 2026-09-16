#![feature(never_type)]

fn main() {
    let val: ! = loop { break break; };
// { dg-error ".E0308." "" { target *-*-* } .-1 }

    loop {
        if true {
            break "asdf";
        } else {
            break 123; // { dg-error ".E0308." "" { target *-*-* } }
        }
    };

    let _: i32 = loop {
        break "asdf"; // { dg-error ".E0308." "" { target *-*-* } }
    };

    let _: i32 = 'outer_loop: loop {
        loop {
            break 'outer_loop "nope"; // { dg-error ".E0308." "" { target *-*-* } }
            break "ok";
        };
    };

    'while_loop: while true { // { dg-warning "" "" { target *-*-* } }
        break;
        break (); // { dg-error ".E0571." "" { target *-*-* } }
        loop {
            break 'while_loop 123;
// { dg-error ".E0571." "" { target *-*-* } .-1 }
            break 456;
            break 789;
        };
    }

    while let Some(_) = Some(()) {
        if break () { // { dg-error ".E0571." "" { target *-*-* } }
        }
    }

    while let Some(_) = Some(()) {
        break None;
// { dg-error ".E0571." "" { target *-*-* } .-1 }
    }

    'while_let_loop: while let Some(_) = Some(()) {
        loop {
            break 'while_let_loop "nope";
// { dg-error ".E0571." "" { target *-*-* } .-1 }
            break 33;
        };
    }

    for _ in &[1,2,3] {
        break (); // { dg-error ".E0571." "" { target *-*-* } }
        break [()];
// { dg-error ".E0571." "" { target *-*-* } .-1 }
    }

    'for_loop: for _ in &[1,2,3] {
        loop {
            break Some(3);
            break 'for_loop Some(17);
// { dg-error ".E0571." "" { target *-*-* } .-1 }
        };
    }

    let _: i32 = 'a: loop {
        let _: () = 'b: loop {
            break ('c: loop {
                break;
                break 'c 123; // { dg-error ".E0308." "" { target *-*-* } }
            });
            break 'a 123;
        };
    };

    loop {
        break (break, break); // { dg-error ".E0308." "" { target *-*-* } }
    };

    loop {
        break;
        break 2; // { dg-error ".E0308." "" { target *-*-* } }
    };

    loop {
        break 2;
        break; // { dg-error ".E0308." "" { target *-*-* } }
        break 4;
    };
}

