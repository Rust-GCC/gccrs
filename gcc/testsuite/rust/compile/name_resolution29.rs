#![feature(no_core)]
#![no_core]

pub mod core {
    #[macro_use]
    pub mod arch {
        #[macro_export]
        macro_rules! asm {
            () => {};
        }
    }
}

pub mod arch {
    pub use ::core::arch::asm;
}

pub mod blah {
    use crate::arch::asm;

    asm! {}
}
