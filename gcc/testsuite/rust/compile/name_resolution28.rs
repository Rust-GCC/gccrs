#![feature(no_core, lang_items)]
#![no_core]

pub mod lateresolve {
    #![lang = "f32"]
    impl f32 {
        pub const RADIX: u32 = 2;
    }

    pub const _: u32 = f32::RADIX;
}
