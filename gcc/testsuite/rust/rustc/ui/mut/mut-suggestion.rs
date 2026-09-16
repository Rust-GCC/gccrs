#[derive(Copy, Clone)]
struct S;

impl S {
    fn mutate(&mut self) {
    }
}

fn func(arg: S) {
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
    arg.mutate();
// { dg-error ".E0596." "" { target *-*-* } .-1 }
}

fn main() {
    let local = S;
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
    local.mutate();
// { dg-error ".E0596." "" { target *-*-* } .-1 }
}

