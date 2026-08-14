enum Test {
    Drill {
        field: i32,
    }
}

fn main() {
    Test::Drill(field: 42);
// { dg-error "" "" { target *-*-* } .-1 }
}

