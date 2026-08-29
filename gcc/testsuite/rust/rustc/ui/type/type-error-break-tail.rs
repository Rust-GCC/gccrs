fn loop_ending() -> i32 {
    loop {
        if false { break; } // { dg-error ".E0308." "" { target *-*-* } }
        return 42;
    }
}

fn main() {}

