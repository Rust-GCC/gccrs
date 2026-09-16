// Test that we get the usual error that we'd get for any other return type and not something about
// diverging functions not being able to return.

fn fail() -> ! {
    return; // { dg-error ".E0069." "" { target *-*-* } }
}

fn main() {
}

