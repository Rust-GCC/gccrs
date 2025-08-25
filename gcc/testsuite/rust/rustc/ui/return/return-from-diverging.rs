// Test that return another type in place of ! raises a type mismatch.

fn fail() -> ! {
    return "wow"; // { dg-error ".E0308." "" { target *-*-* } }
}

fn main() {
}

