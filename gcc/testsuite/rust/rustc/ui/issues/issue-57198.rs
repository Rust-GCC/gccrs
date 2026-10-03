mod m {
    pub fn r#for() {}
}

fn main() {
    m::for();
// { dg-error "" "" { target *-*-* } .-1 }
}

