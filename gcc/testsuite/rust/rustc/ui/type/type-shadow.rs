fn main() {
    type X = isize;
    type Y = X;
    if true {
        type X = &'static str;
        let y: Y = "hello"; // { dg-error ".E0308." "" { target *-*-* } }
    }
}

