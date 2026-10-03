// After #39485, this test used to pass, but that change was reverted
// due to numerous inference failures like #39808, so it now fails
// again. #39485 made it so that diverging types never propagate
// upward; but we now do propagate such types upward in many more
// cases.

fn g() {
    &panic!() // { dg-error ".E0308." "" { target *-*-* } }
}

fn f() -> isize {
    (return 1, return 2) // { dg-error ".E0308." "" { target *-*-* } }
}

fn main() {}

