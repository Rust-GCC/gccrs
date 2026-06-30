#![feature(no_core, lang_items)]
#![no_core]

#[lang = "sized"]
pub trait Sized {}

trait TraitA {
    fn do_a(&self) -> i32;
}

trait TraitB {
    fn do_b(&self) -> i32;
}

trait TraitC {}

struct StructX {
    val: i32,
}
struct StructY {
    val: i32,
}

impl TraitA for StructX {
    fn do_a(&self) -> i32 {
        self.val
    }
}

impl TraitB for StructX {
    fn do_b(&self) -> i32 {
        self.val * 2
    }
}

impl TraitC for StructX {}

impl TraitA for StructY {
    fn do_a(&self) -> i32 {
        self.val + 10
    }
}

fn main() -> i32 {
    let x = StructX { val: 0 };
    let x1 = StructX { val: 1 };
    let x2 = StructX { val: 2 };
    let y = StructY { val: 3 };

    let _dyn_a_x: &dyn TraitC = &x;

    let dyn_a_x1: &dyn TraitA = &x1;

    let dyn_a_x2: &dyn TraitA = &x2;

    let dyn_a_y: &dyn TraitA = &y;

    let dyn_b_x1: &dyn TraitB = &x1;

    if x1.val != 1 || x2.val != 2 || y.val != 3 {
        1
    } else if dyn_a_x1.do_a() != 1 {
        2
    } else if dyn_a_x2.do_a() != 2 {
        3
    } else if dyn_a_y.do_a() != 13 {
        4
    } else if dyn_b_x1.do_b() != 2 {
        5
    } else {
        0
    }
}
