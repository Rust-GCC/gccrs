fn main() {
    (0..)
        .map(
            #[target_feature(enable = "")]
// { dg-error "" "" { target *-*-* } .-1 }
            #[track_caller]
            |_| (),
// { dg-note "" "" { target *-*-* } .-1 }
        )
        .next();
}

