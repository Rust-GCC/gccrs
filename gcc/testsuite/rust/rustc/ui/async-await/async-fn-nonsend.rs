// edition:2018
// compile-flags: --crate-type lib

use std::{cell::RefCell, fmt::Debug, rc::Rc};

fn non_sync() -> impl Debug {
    RefCell::new(())
}

fn non_send() -> impl Debug {
    Rc::new(())
}

fn take_ref<T>(_: &T) {}

async fn fut() {}

async fn fut_arg<T>(_: T) {}

async fn local_dropped_before_await() {
    // FIXME: it'd be nice for this to be allowed in a `Send` `async fn`
    let x = non_send();
    drop(x);
    fut().await;
}

async fn non_send_temporary_in_match() {
    // We could theoretically make this work as well (produce a `Send` future)
    // for scrutinees / temporaries that can or will
    // be dropped prior to the match body
    // (e.g. `Copy` types).
    match Some(non_send()) {
        Some(_) => fut().await,
        None => {}
    }
}

async fn non_sync_with_method_call() {
    // FIXME: it'd be nice for this to work.
    let f: &mut std::fmt::Formatter = panic!();
    if non_sync().fmt(f).unwrap() == () {
        fut().await;
    }
}

fn assert_send(_: impl Send) {}

pub fn pass_assert() {
    assert_send(local_dropped_before_await());
// { dg-error "" "" { target *-*-* } .-1 }
    assert_send(non_send_temporary_in_match());
// { dg-error "" "" { target *-*-* } .-1 }
    assert_send(non_sync_with_method_call());
// { dg-error "" "" { target *-*-* } .-1 }
}

