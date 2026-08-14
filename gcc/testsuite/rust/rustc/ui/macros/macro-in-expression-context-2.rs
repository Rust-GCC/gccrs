macro_rules! empty { () => () }

fn main() {
    match 42 {
        _ => { empty!() }
// { dg-error "" "" { target *-*-* } .-1 }
    };
}

