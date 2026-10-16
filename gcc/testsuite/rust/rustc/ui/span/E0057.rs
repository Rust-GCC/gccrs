fn main() {
    let f = |x| x * 3;
    let a = f(); // { dg-error ".E0057." "" { target *-*-* } }
    let b = f(4);
    let c = f(2, 3); // { dg-error ".E0057." "" { target *-*-* } }
}

