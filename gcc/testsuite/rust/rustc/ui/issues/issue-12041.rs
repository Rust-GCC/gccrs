use std::sync::mpsc::channel;
use std::thread;

fn main() {
    let (tx, rx) = channel();
    let _t = thread::spawn(move|| -> () {
        loop {
            let tx = tx;
// { dg-error ".E0382." "" { target *-*-* } .-1 }
            tx.send(1);
        }
    });
}

