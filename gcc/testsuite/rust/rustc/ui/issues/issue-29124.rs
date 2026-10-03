struct Ret;
struct Obj;

impl Obj {
    fn func() -> Ret {
        Ret
    }
}

fn func() -> Ret {
    Ret
}

fn main() {
    Obj::func.x();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
    func.x();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
}

