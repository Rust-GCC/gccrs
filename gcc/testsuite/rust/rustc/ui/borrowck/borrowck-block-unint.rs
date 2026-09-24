fn force<F>(f: F) where F: FnOnce() { f(); }
fn main() {
    let x: isize;
    force(|| {  // { dg-error ".E0381." "" { target *-*-* } }
        println!("{}", x);
    });
}

