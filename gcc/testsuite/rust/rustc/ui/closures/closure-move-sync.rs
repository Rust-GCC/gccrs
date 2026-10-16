use std::thread;
use std::sync::mpsc::channel;

fn bar() {
    let (send, recv) = channel();
    let t = thread::spawn(|| {
        recv.recv().unwrap();
// { dg-error ".E0277." "" { target *-*-* } .-2 }
    });

    send.send(());

    t.join().unwrap();
}

fn foo() {
    let (tx, _rx) = channel();
    thread::spawn(|| tx.send(()).unwrap());
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn main() {}

