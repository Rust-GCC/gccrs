#![feature(start)]

#[start]
#[track_caller] // { dg-error "" "" { target *-*-* } }
fn start(_argc: isize, _argv: *const *const u8) -> isize {
    panic!("{}: oh no", std::panic::Location::caller());
}

