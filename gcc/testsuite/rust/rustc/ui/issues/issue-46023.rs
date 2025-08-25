fn main() {
    let x = 0;

    (move || {
        x = 1;
// { dg-error ".E0594." "" { target *-*-* } .-1 }
    })()
}

