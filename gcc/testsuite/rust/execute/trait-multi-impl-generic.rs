#![feature(no_core, lang_items)]
#![no_core]

#[lang = "sized"]
pub trait Sized {}

trait TraitA {
    fn do_a(&self) -> i32;
}

pub struct Container<T> {
    pub val: i32,
    pub _marker: T,
}

impl TraitA for Container<i32> {
    fn do_a(&self) -> i32 {
        self.val * 10
    }
}

impl TraitA for Container<u32> {
    fn do_a(&self) -> i32 {
        self.val * 20
    }
}

fn main() -> i32 {
    let a = Container {
        val: 10,
        _marker: 0_i32,
    };
    let b = Container {
        val: 20,
        _marker: 0_u32,
    };

    let dyn_a = &a as &dyn TraitA;
    let dyn_b = &b as &dyn TraitA;

    if dyn_a.do_a() == 100 && dyn_b.do_a() == 400 {
        0
    } else {
        1
    }
}
