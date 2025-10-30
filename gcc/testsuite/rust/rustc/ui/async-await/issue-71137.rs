// edition:2018

use std::future::Future;
use std::sync::Mutex;

fn fake_spawn<F: Future + Send + 'static>(f: F) { }

async fn wrong_mutex() {
  let m = Mutex::new(1);
  {
    let mut guard = m.lock().unwrap();
    (async { "right"; }).await;
    *guard += 1;
  }

  (async { "wrong"; }).await;
}

fn main() {
  fake_spawn(wrong_mutex()); // { dg-error "" "" { target *-*-* } }
}

