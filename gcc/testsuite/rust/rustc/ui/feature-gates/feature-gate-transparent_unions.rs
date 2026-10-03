#[repr(transparent)]
union OkButUnstableUnion { // { dg-error ".E0658." "" { target *-*-* } }
    field: u8,
    zst: (),
}

fn main() {}

