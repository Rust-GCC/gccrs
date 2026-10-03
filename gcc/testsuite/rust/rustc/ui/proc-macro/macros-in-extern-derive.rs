extern {
    #[derive(Copy)] // { dg-error ".E0774." "" { target *-*-* } }
    fn f();
}

fn main() {}

