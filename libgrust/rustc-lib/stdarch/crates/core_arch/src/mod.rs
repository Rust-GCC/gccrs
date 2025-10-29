//! `core_arch`

#[macro_use]
mod macros;

#[cfg(any(target_arch = "arm", target_arch = "aarch64", doc))]
mod acle;

mod simd;

#[doc(include = "core_arch_docs.md")]
#[stable(feature = "simd_arch", since = "1.27.0")]
pub mod arch {
    /// Platform-specific intrinsics for the `x86` platform.
    ///
    /// See the [module documentation](../index.html) for more details.
    #[cfg(any(target_arch = "x86", doc))]
    #[doc(cfg(target_arch = "x86"))]
    #[stable(feature = "simd_x86", since = "1.27.0")]
    pub mod x86 {
        #[stable(feature = "simd_x86", since = "1.27.0")]
        pub use crate::core_arch::x86::*;
    }

    /// Platform-specific intrinsics for the `x86_64` platform.
    ///
    /// See the [module documentation](../index.html) for more details.
    #[cfg(any(target_arch = "x86_64", doc))]
    #[doc(cfg(target_arch = "x86_64"))]
    #[stable(feature = "simd_x86", since = "1.27.0")]
    pub mod x86_64 {
        #[stable(feature = "simd_x86", since = "1.27.0")]
        pub use crate::core_arch::x86::*;
        #[stable(feature = "simd_x86", since = "1.27.0")]
        pub use crate::core_arch::x86_64::*;
    }

    /// Platform-specific intrinsics for the `arm` platform.
    ///
    /// See the [module documentation](../index.html) for more details.
    #[cfg(any(target_arch = "arm", doc))]
    #[doc(cfg(target_arch = "arm"))]
    #[unstable(feature = "stdsimd", issue = "27731")]
    pub mod arm {
        pub use crate::core_arch::arm::*;
    }

    /// Platform-specific intrinsics for the `aarch64` platform.
    ///
    /// See the [module documentation](../index.html) for more details.
    #[cfg(any(target_arch = "aarch64", doc))]
    #[doc(cfg(target_arch = "aarch64"))]
    #[unstable(feature = "stdsimd", issue = "27731")]
    pub mod aarch64 {
        pub use crate::core_arch::{aarch64::*, arm::*};
    }

    /// Platform-specific intrinsics for the `wasm32` platform.
    ///
    /// This module provides intrinsics specific to the WebAssembly
    /// architecture. Here you'll find intrinsics necessary for leveraging
    /// WebAssembly proposals such as [atomics] and [simd]. These proposals are
    /// evolving over time and as such the support here is unstable and requires
    /// the nightly channel. As WebAssembly proposals stabilize these functions
    /// will also become stable.
    ///
    /// [atomics]: https://github.com/webassembly/threads
    /// [simd]: https://github.com/webassembly/simd
    ///
    /// See the [module documentation](../index.html) for general information
    /// about the `arch` module and platform intrinsics.
    ///
    /// ## Atomics
    ///
    /// The [threads proposal][atomics] for WebAssembly adds a number of
    /// instructions for dealing with multithreaded programs. Atomic
    /// instructions can all be generated through `std::sync::atomic` types, but
    /// some instructions have no equivalent in Rust such as
    /// `memory.atomic.notify` so this module will provide these intrinsics.
    ///
    /// At this time, however, these intrinsics are only available **when the
    /// standard library itself is compiled with atomics**. Compiling with
    /// atomics is not enabled by default and requires passing
    /// `-Ctarget-feature=+atomics` to rustc. The standard library shipped via
    /// `rustup` is not compiled with atomics. To get access to these intrinsics
    /// you'll need to compile the standard library from source with the
    /// requisite compiler flags.
    ///
    /// ## SIMD
    ///
    /// The [simd proposal][simd] for WebAssembly adds a new `v128` type for a
    /// 128-bit SIMD register. It also adds a large array of instructions to
    /// operate on the `v128` type to perform data processing. The SIMD proposal
    /// has been in progress for quite some time and many instructions have come
    /// and gone. This module attempts to keep up with the proposal, but if you
    /// notice anything awry please feel free to [open an
    /// issue](https://github.com/rust-lang/stdarch/issues/new).
    ///
    /// It's important to be aware that the current state of development of SIMD
    /// in WebAssembly is still somewhat early days. There's lots of pieces to
    /// demo and prototype with, but discussions and support are still in
    /// progress. There's a number of pitfalls and gotchas in various places,
    /// which will attempt to be documented here, but there may be others
    /// lurking!
    ///
    /// Using SIMD is intended to be similar to as you would on `x86_64`, for
    /// example. You'd write a function such as:
    ///
    /// ```rust,ignore
    /// #[cfg(target_arch = "wasm32")]
    /// #[target_feature(enable = "simd128")]
    /// unsafe fn uses_simd() {
    ///     use std::arch::wasm32::*;
    ///     // ...
    /// }
    /// ```
    ///
    /// Unlike `x86_64`, however, WebAssembly does not currently have dynamic
    /// detection at runtime as to whether SIMD is supported (this is one of the
    /// motivators for the [conditional sections proposal][condsections], but
    /// that is still pretty early days). This means that your binary will
    /// either have SIMD and can only run on engines which support SIMD, or it
    /// will not have SIMD at all. For compatibility the standard library itself
    /// does not use any SIMD internally. Determining how best to ship your
    /// WebAssembly binary with SIMD is largely left up to you as it can can be
    /// pretty nuanced depending on your situation.
    ///
    /// [condsections]: https://github.com/webassembly/conditional-sections
    ///
    /// To enable SIMD support at compile time you need to do one of two things:
    ///
    /// * First you can annotate functions with `#[target_feature(enable =
    ///   "simd128")]`. This causes just that one function to have SIMD support
    ///   available to it, and intrinsics will get inlined as usual in this
    ///   situation.
    ///
    /// * Second you can compile your program with `-Ctarget-feature=+simd128`.
    ///   This compilation flag blanket enables SIMD support for your entire
    ///   compilation. Note that this does not include the standard library
    ///   unless you recompile the standard library.
    ///
    /// If you enable SIMD via either of these routes then you'll have a
    /// WebAssembly binary that uses SIMD instructions, and you'll need to ship
    /// that accordingly. Also note that if you call SIMD intrinsics but don't
    /// enable SIMD via either of these mechanisms, you'll still have SIMD
    /// generated in your program. This means to generate a binary without SIMD
    /// you'll need to avoid both options above plus calling into any intrinsics
    /// in this module.
    ///
    /// > **Note**: Due to
    /// > [rust-lang/rust#74320](https://github.com/rust-lang/rust/issues/74320)
    /// > it's recommended to compile your entire program with SIMD support
    /// > (using `RUSTFLAGS`) or otherwise functions may not be inlined
    /// > correctly.
    ///
    /// > **Note**: LLVM's SIMD support is actually split into two features:
    /// > `simd128` and `unimplemented-simd128`. Rust code can enable `simd128`
    /// > with `#[target_feature]` (and test for it with `#[cfg(target_feature =
    /// > "simd128")]`, but it cannot enable `unimplemented-simd128`. The only
    /// > way to enable this feature is to compile with
    /// > `-Ctarget-feature=+simd128,+unimplemented-simd128`. This second
    /// > feature enables more recent instructions implemented in LLVM which
    /// > haven't always had enough time to make their way to runtimes.
    #[cfg(any(target_arch = "wasm32", doc))]
    #[doc(cfg(target_arch = "wasm32"))]
    #[stable(feature = "simd_wasm32", since = "1.33.0")]
    pub mod wasm32 {
        #[stable(feature = "simd_wasm32", since = "1.33.0")]
        pub use crate::core_arch::wasm32::*;
    }

    /// Platform-specific intrinsics for the `mips` platform.
    ///
    /// See the [module documentation](../index.html) for more details.
    #[cfg(any(target_arch = "mips", doc))]
    #[doc(cfg(target_arch = "mips"))]
    #[unstable(feature = "stdsimd", issue = "27731")]
    pub mod mips {
        pub use crate::core_arch::mips::*;
    }

    /// Platform-specific intrinsics for the `mips64` platform.
    ///
    /// See the [module documentation](../index.html) for more details.
    #[cfg(any(target_arch = "mips64", doc))]
    #[doc(cfg(target_arch = "mips64"))]
    #[unstable(feature = "stdsimd", issue = "27731")]
    pub mod mips64 {
        pub use crate::core_arch::mips::*;
    }

    /// Platform-specific intrinsics for the `PowerPC` platform.
    ///
    /// See the [module documentation](../index.html) for more details.
    #[cfg(any(target_arch = "powerpc", doc))]
    #[doc(cfg(target_arch = "powerpc"))]
    #[unstable(feature = "stdsimd", issue = "27731")]
    pub mod powerpc {
        pub use crate::core_arch::powerpc::*;
    }

    /// Platform-specific intrinsics for the `PowerPC64` platform.
    ///
    /// See the [module documentation](../index.html) for more details.
    #[cfg(any(target_arch = "powerpc64", doc))]
    #[doc(cfg(target_arch = "powerpc64"))]
    #[unstable(feature = "stdsimd", issue = "27731")]
    pub mod powerpc64 {
        pub use crate::core_arch::powerpc64::*;
    }

    /// Platform-specific intrinsics for the `NVPTX` platform.
    ///
    /// See the [module documentation](../index.html) for more details.
    #[cfg(any(target_arch = "nvptx", target_arch = "nvptx64", doc))]
    #[doc(cfg(any(target_arch = "nvptx", target_arch = "nvptx64")))]
    #[unstable(feature = "stdsimd", issue = "27731")]
    pub mod nvptx {
        pub use crate::core_arch::nvptx::*;
    }
}

mod simd_llvm;

#[cfg(any(target_arch = "x86", target_arch = "x86_64", doc))]
#[doc(cfg(any(target_arch = "x86", target_arch = "x86_64")))]
mod x86;
#[cfg(any(target_arch = "x86_64", doc))]
#[doc(cfg(target_arch = "x86_64"))]
mod x86_64;

#[cfg(any(target_arch = "aarch64", doc))]
#[doc(cfg(target_arch = "aarch64"))]
mod aarch64;
#[cfg(any(target_arch = "arm", target_arch = "aarch64", doc))]
#[doc(cfg(any(target_arch = "arm", target_arch = "aarch64")))]
mod arm;

#[cfg(any(target_arch = "wasm32", doc))]
#[doc(cfg(target_arch = "wasm32"))]
mod wasm32;

#[cfg(any(target_arch = "mips", target_arch = "mips64", doc))]
#[doc(cfg(any(target_arch = "mips", target_arch = "mips64")))]
mod mips;

#[cfg(any(target_arch = "powerpc", target_arch = "powerpc64", doc))]
#[doc(cfg(any(target_arch = "powerpc", target_arch = "powerpc64")))]
mod powerpc;

#[cfg(any(target_arch = "powerpc64", doc))]
#[doc(cfg(target_arch = "powerpc64"))]
mod powerpc64;

#[cfg(any(target_arch = "nvptx", target_arch = "nvptx64", doc))]
#[doc(cfg(any(target_arch = "nvptx", target_arch = "nvptx64")))]
mod nvptx;
