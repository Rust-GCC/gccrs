// check-pass
#![allow(dead_code)]
// pretty-expanded FIXME #23616

trait PoolManager {
    type C;
    fn dummy(&self) { }
}

struct InnerPool<M: PoolManager> {
    manager: M,
}

fn main() {}

