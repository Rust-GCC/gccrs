#[track_caller]
extern "C" fn f() {}
// { dg-error ".E0737." "" { target *-*-* } .-2 }

extern "C" {
    #[track_caller]
    fn g();
// { dg-error ".E0737." "" { target *-*-* } .-2 }
}

fn main() {}

