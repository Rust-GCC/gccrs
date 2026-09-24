// Check that we get nice suggestions when attempting a chained comparison.

fn comp1() {
    1 < 2 <= 3; // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

fn comp2() {
    1 < 2 < 3; // { dg-error "" "" { target *-*-* } }
}

fn comp3() {
    1 <= 2 < 3; // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

fn comp4() {
    1 <= 2 <= 3; // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

fn comp5() {
    1 > 2 >= 3; // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

fn comp6() {
    1 > 2 > 3; // { dg-error "" "" { target *-*-* } }
}

fn comp7() {
    1 >= 2 > 3; // { dg-error "" "" { target *-*-* } }
}

fn comp8() {
    1 >= 2 >= 3; // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

fn comp9() {
    1 == 2 < 3; // { dg-error "" "" { target *-*-* } }
}

fn comp10() {
    1 > 2 == false; // { dg-error "" "" { target *-*-* } }
}

fn comp11() {
    1 == 2 == 3; // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

fn main() {}

