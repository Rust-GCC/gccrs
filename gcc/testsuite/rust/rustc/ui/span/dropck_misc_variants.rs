// check that dropck does the right thing with misc. Ty variants

use std::fmt;
struct NoisyDrop<T: fmt::Debug>(T);
impl<T: fmt::Debug> Drop for NoisyDrop<T> {
    fn drop(&mut self) {
        let _ = vec!["0wned"];
        println!("dropping {:?}", self.0)
    }
}

trait Associator {
    type As;
}
impl<T: fmt::Debug> Associator for T {
    type As = NoisyDrop<T>;
}
struct Wrap<A: Associator>(<A as Associator>::As);

fn projection() {
    let (_w, bomb);
    bomb = vec![""];
    _w = Wrap::<&[&str]>(NoisyDrop(&bomb));
}
// { dg-error ".E0597." "" { target *-*-* } .-2 }

fn closure() {
    let (_w,v);
    v = vec![""];
    _w = {
        let u = NoisyDrop(&v);
// { dg-error ".E0597." "" { target *-*-* } .-1 }
        move || u.0.len()
    };
}

fn main() { closure(); projection() }

