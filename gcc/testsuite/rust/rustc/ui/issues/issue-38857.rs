fn main() {
    let a = std::sys::imp::process::process_common::StdioPipes { ..panic!() };
// { dg-error ".E0603." "" { target *-*-* } .-1 }
// { dg-error ".E0603." "" { target *-*-* } .-2 }
}

