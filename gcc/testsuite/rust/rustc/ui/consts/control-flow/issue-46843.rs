enum Thing {
    This,
    That,
}

fn non_const() -> Thing {
    Thing::This
}

pub const Q: i32 = match non_const() {
// { dg-error ".E0015." "" { target *-*-* } .-1 }
    Thing::This => 1,
    Thing::That => 0
};

fn main() {}

