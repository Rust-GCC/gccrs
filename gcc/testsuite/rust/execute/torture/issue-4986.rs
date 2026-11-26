#![feature(no_core)]
#![no_core]
#![feature(lang_items)]

#[lang = "sized"]
pub trait Sized {}

pub trait Foo {
    type bar;
    fn bar(&self) -> i32 {
        1
    }
}

// No override: must use the trait's default `bar`
impl Foo for char {
    type bar = u8;
}

// Override: must select the impl's `fn bar`, not `type bar`
impl Foo for bool {
    type bar = u8;
    fn bar(&self) -> i32 {
        2
    }
}

fn main() -> i32 {
    let a = 'a'.bar();
    let b = true.bar();
    
    a + b - 3
}
