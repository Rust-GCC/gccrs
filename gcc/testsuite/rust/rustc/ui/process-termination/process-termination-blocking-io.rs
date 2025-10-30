// program should terminate even if a thread is blocked on I/O.
// https://github.com/fortanix/rust-sgx/issues/109

// run-pass
// ignore-emscripten no threads support

use std::{net::TcpListener, sync::mpsc, thread};

fn main() {
    let (tx, rx) = mpsc::channel();
    thread::spawn(move || {
        let listen = TcpListener::bind("0.0.0.0:0").unwrap();
        tx.send(()).unwrap();
        while let Ok(_) = listen.accept() {}
    });
    rx.recv().unwrap();
    for _ in 0..3 { thread::yield_now(); }
    println!("Exiting main thread");
}

