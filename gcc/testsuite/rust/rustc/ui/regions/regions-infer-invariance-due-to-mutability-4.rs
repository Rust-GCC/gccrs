struct Invariant<'a> {
    f: Box<dyn FnOnce() -> *mut &'a isize + 'static>,
}

fn to_same_lifetime<'r>(b_isize: Invariant<'r>) {
    let bj: Invariant<'r> = b_isize;
}

fn to_longer_lifetime<'r>(b_isize: Invariant<'r>) -> Invariant<'static> {
    b_isize // { dg-error ".E0308." "" { target *-*-* } }
}

fn main() {
}

