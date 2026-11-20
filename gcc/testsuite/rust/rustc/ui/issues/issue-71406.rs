use std::sync::mpsc;

fn main() {
    let (tx, rx) = mpsc::channel::new(1);
// { dg-error ".E0433." "" { target *-*-* } .-1 }
}

