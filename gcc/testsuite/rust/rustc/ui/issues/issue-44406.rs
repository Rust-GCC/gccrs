macro_rules! foo {
    ($rest: tt) => {
        bar(baz: $rest)
    }
}

fn main() {
    foo!(true); // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
}

