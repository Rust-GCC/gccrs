fn main() {
    let _: char = '';
// { dg-error "" "" { target *-*-* } .-1 }
    let _: char = '\u{}';
// { dg-error "" "" { target *-*-* } .-1 }

    // Next two are OK, but may befool error recovery
    let _ = '/';
    let _ = b'/';

    let _ = ' hello // here's a comment
// { dg-error ".E0762." "" { target *-*-* } .-1 }
}

