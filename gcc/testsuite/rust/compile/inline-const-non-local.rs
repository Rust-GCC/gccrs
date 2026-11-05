#![feature(no_core)]
#![no_core]

const K: i32 = 1;
static S: i32 = 2;

fn generic<const N: usize>() -> usize {
    let v = const { N * 2 };
    v
}

fn items() -> i32 {
    let v = const { K + S };
    v
}

fn block_local() -> i32 {
    let v = const {
        let y = 2;
        y + 1
    };
    v
}

fn nested_fn_local() -> i32 {
    let v = const {
        const fn inner() -> i32 {
            let z = 3;
            z
        }
        inner()
    };
    v
}

fn main() {
    generic::<3>();
    items();
    block_local();
    nested_fn_local();
}
