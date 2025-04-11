// %d// const X: i32 = const { 15 + 14 };

// #![feature(rustc_attrs)]

// #[rustc_builtin_macro]
// macro_rules! llvm_asm {
//     () => {};
// }

// pub fn black_box<T>(mut dummy: T) -> T {
//     // We need to "use" the argument in some way LLVM can't introspect, and on
//     // targets that support it we can typically leverage inline assembly to do
//     // this. LLVM's interpretation of inline assembly is that it's, well, a black
//     // box. This isn't the greatest implementation since it probably deoptimizes
//     // more than we want, but it's so far good enough.

//     #[cfg(not(miri))] // This is just a hint, so it is fine to skip in Miri.
//     // SAFETY: the inline assembly is a no-op.
//     unsafe {
//         // FIXME: Cannot use `asm!` because it doesn't support MIPS and other architectures.
//         llvm_asm!("" : : "r"(&mut dummy) : "memory" : "volatile");
//     }

//     dummy
// }

macro_rules! generate_pattern_iterators {
    {
        $(#[$forward_iterator_attribute:meta])*
    } => {
    }
}

generate_pattern_iterators! {
    /// Created with the method [`split`].
}
