// Check notes are placed on an assignment that can actually precede the current assignment
// Don't emit a first assignment for assignment in a loop.

fn test() {
    let x;
    if true {
        x = 1;
    } else {
        x = 2;
        x = 3;      // { dg-error ".E0384." "" { target *-*-* } }
    }
}

fn test_in_loop() {
    loop {
        let x;
        if true {
            x = 1;
        } else {
            x = 2;
            x = 3;      // { dg-error ".E0384." "" { target *-*-* } }
        }
    }
}

fn test_using_loop() {
    let x;
    loop {
        if true {
            x = 1;      // { dg-error ".E0384." "" { target *-*-* } }
        } else {
            x = 2;      // { dg-error ".E0384." "" { target *-*-* } }
        }
    }
}

fn main() {}

