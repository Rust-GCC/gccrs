// compile-flags: -Zsave-analysis

fn main() {
    match 'a' {
        char{ch} => true
// { dg-error ".E0574." "" { target *-*-* } .-1 }
    };
}

