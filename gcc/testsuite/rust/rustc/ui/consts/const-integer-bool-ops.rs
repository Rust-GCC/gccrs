const X: usize = 42 && 39;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
// { dg-error ".E0308." "" { target *-*-* } .-5 }
// { dg-error ".E0308." "" { target *-*-* } .-6 }
const ARR: [i32; X] = [99; 34];

const X1: usize = 42 || 39;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
// { dg-error ".E0308." "" { target *-*-* } .-5 }
// { dg-error ".E0308." "" { target *-*-* } .-6 }
const ARR1: [i32; X1] = [99; 47];

const X2: usize = -42 || -39;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
// { dg-error ".E0308." "" { target *-*-* } .-5 }
// { dg-error ".E0308." "" { target *-*-* } .-6 }
const ARR2: [i32; X2] = [99; 18446744073709551607];

const X3: usize = -42 && -39;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
// { dg-error ".E0308." "" { target *-*-* } .-5 }
// { dg-error ".E0308." "" { target *-*-* } .-6 }
const ARR3: [i32; X3] = [99; 6];

const Y: usize = 42.0 == 42.0;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
const ARRR: [i32; Y] = [99; 1];

const Y1: usize = 42.0 >= 42.0;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
const ARRR1: [i32; Y1] = [99; 1];

const Y2: usize = 42.0 <= 42.0;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
const ARRR2: [i32; Y2] = [99; 1];

const Y3: usize = 42.0 > 42.0;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
const ARRR3: [i32; Y3] = [99; 0];

const Y4: usize = 42.0 < 42.0;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
const ARRR4: [i32; Y4] = [99; 0];

const Y5: usize = 42.0 != 42.0;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
const ARRR5: [i32; Y5] = [99; 0];

fn main() {
    let _ = ARR;
    let _ = ARRR;
    let _ = ARRR1;
    let _ = ARRR2;
    let _ = ARRR3;
    let _ = ARRR4;
    let _ = ARRR5;
}

