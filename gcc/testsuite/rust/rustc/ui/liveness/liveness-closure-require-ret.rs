fn force<F>(f: F) -> isize where F: FnOnce() -> isize { f() }
fn main() { println!("{}", force(|| {})); } // { dg-error ".E0308." "" { target *-*-* } }

