fn main() {
    let mut x = 0;
    || {
        || {
// { dg-error "" "" { target *-*-* } .-1 }
            let _y = &mut x;
        }
    };
}

