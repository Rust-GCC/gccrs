fn test_drop_replace() {
    let b: Box<isize>;
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
    b = Box::new(1);    // { dg-note "" "" { target *-*-* } }
    b = Box::new(2);    // { dg-error ".E0384." "" { target *-*-* } }
// { dg-note "" "" { target *-*-* } .-2 }
}

fn test_call() {
    let b = Box::new(1);    // { dg-note "" "" { target *-*-* } }
// { help "" "" { target *-*-* } .-2 }
// { suggestion "" "" { target *-*-* } .-3 }
    b = Box::new(2);        // { dg-error ".E0384." "" { target *-*-* } }
// { dg-note "" "" { target *-*-* } .-2 }
}

fn test_args(b: Box<i32>) {  // { help "" "" { target *-*-* } }
// { suggestion "" "" { target *-*-* } .-2 }
    b = Box::new(2);            // { dg-error ".E0384." "" { target *-*-* } }
// { dg-note "" "" { target *-*-* } .-2 }
}

fn main() {}

