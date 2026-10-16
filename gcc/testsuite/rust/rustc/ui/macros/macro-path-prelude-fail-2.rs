mod m {
    fn check() {
        Result::Ok!(); // { dg-error ".E0433." "" { target *-*-* } }
    }
}

fn main() {}

