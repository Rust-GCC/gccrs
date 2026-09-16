fn main () {
    loop {
        break 'a;
// { dg-error ".E0426." "" { target *-*-* } .-1 }
    }
}

