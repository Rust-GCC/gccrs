#![feature(no_core, lang_items)]
#![no_core]

#[lang = "sized"]
pub trait Sized {}

trait TraitA {
    fn do_a(&self) -> i32;
}

mod mod_a {
    use super::TraitA;

    pub struct StructX<T> {
        pub val: i32,
        _marker: T,
    }

    impl TraitA for StructX<i32> {
        fn do_a(&self) -> i32 {
            self.val * 10
        }
    }
    impl TraitA for StructX<u32> {
        fn do_a(&self) -> i32 {
            self.val * 1000
        }
    }
}

mod mod_b {
    use super::TraitA;

    pub struct StructX<T> {
        pub val: i32,
        _marker: T,
    }

    impl TraitA for StructX<u32> {
        fn do_a(&self) -> i32 {
            self.val * 20
        }
    }
    impl TraitA for StructX<i32> {
        fn do_a(&self) -> i32 {
            self.val * 2000
        }
    }
}

fn main() -> i32 {
    let a = mod_a::StructX {
        val: 10,
        _marker: 0_i32,
    };
    let b = mod_b::StructX {
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
