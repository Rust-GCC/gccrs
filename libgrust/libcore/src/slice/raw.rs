//! Free functions to create `&[T]` and `&mut [T]`.

use crate::array;
use crate::intrinsics::is_aligned_and_not_null;
use crate::mem;
use crate::ptr;

/// Forms a slice from a pointer and a length.
///
/// The `len` argument is the number of **elements**, not the number of bytes.
///
/// # Safety
///
/// Behavior is undefined if any of the following conditions are violated:
///
/// * `data` must be [valid] for reads for `len * mem::size_of::<T>()` many bytes,
///   and it must be properly aligned. This means in particular:
///
///     * The entire memory range of this slice must be contained within a single allocated object!
///       Slices can never span across multiple allocated objects. See [below](#incorrect-usage)
///       for an example incorrectly not taking this into account.
///     * `data` must be non-null and aligned even for zero-length slices. One
///       reason for this is that enum layout optimizations may rely on references
///       (including slices of any length) being aligned and non-null to distinguish
///       them from other data. You can obtain a pointer that is usable as `data`
///       for zero-length slices using [`NonNull::dangling()`].
///
/// * `data` must point to `len` consecutive properly initialized values of type `T`.
///
/// * The memory referenced by the returned slice must not be mutated for the duration
///   of lifetime `'a`, except inside an `UnsafeCell`.
///
/// * The total size `len * mem::size_of::<T>()` of the slice must be no larger than `isize::MAX`.
///   See the safety documentation of [`pointer::offset`].
///
/// # Caveat
///
/// The lifetime for the returned slice is inferred from its usage. To
/// prevent accidental misuse, it's suggested to tie the lifetime to whichever
/// source lifetime is safe in the context, such as by providing a helper
/// function taking the lifetime of a host value for the slice, or by explicit
/// annotation.
///
/// # Examples
///
/// ```
/// use std::slice;
///
/// // manifest a slice for a single element
/// let x = 42;
/// let ptr = &x as *const _;
/// let slice = unsafe { slice::from_raw_parts(ptr, 1) };
/// assert_eq!(slice[0], 42);
/// ```
///
/// ### Incorrect usage
///
/// The following `join_slices` function is **unsound** ⚠️
///
/// ```rust,no_run
/// use std::slice;
///
/// fn join_slices<'a, T>(fst: &'a [T], snd: &'a [T]) -> &'a [T] {
///     let fst_end = fst.as_ptr().wrapping_add(fst.len());
///     let snd_start = snd.as_ptr();
///     assert_eq!(fst_end, snd_start, "Slices must be contiguous!");
///     unsafe {
///         // The assertion above ensures `fst` and `snd` are contiguous, but they might
///         // still be contained within _different allocated objects_, in which case
///         // creating this slice is undefined behavior.
///         slice::from_raw_parts(fst.as_ptr(), fst.len() + snd.len())
///     }
/// }
///
/// fn main() {
///     // `a` and `b` are different allocated objects...
///     let a = 42;
///     let b = 27;
///     // ... which may nevertheless be laid out contiguously in memory: | a | b |
///     let _ = join_slices(slice::from_ref(&a), slice::from_ref(&b)); // UB
/// }
/// ```
///
/// [valid]: ptr#safety
/// [`NonNull::dangling()`]: ptr::NonNull::dangling
/// [`pointer::offset`]: ../../std/primitive.pointer.html#method.offset
#[inline]
#[stable(feature = "rust1", since = "1.0.0")]
pub unsafe fn from_raw_parts<'a, T>(data: *const T, len: usize) -> &'a [T] {
    debug_assert!(is_aligned_and_not_null(data), "attempt to create unaligned or null slice");
    debug_assert!(
        mem::size_of::<T>().saturating_mul(len) <= isize::MAX as usize,
        "attempt to create slice covering at least half the address space"
    );
    // SAFETY: the caller must uphold the safety contract for `from_raw_parts`.
    unsafe { &*ptr::slice_from_raw_parts(data, len) }
}

/// Performs the same functionality as [`from_raw_parts`], except that a
/// mutable slice is returned.
///
/// # Safety
///
/// Behavior is undefined if any of the following conditions are violated:
///
/// * `data` must be [valid] for boths reads and writes for `len * mem::size_of::<T>()` many bytes,
///   and it must be properly aligned. This means in particular:
///
///     * The entire memory range of this slice must be contained within a single allocated object!
///       Slices can never span across multiple allocated objects.
///     * `data` must be non-null and aligned even for zero-length slices. One
///       reason for this is that enum layout optimizations may rely on references
///       (including slices of any length) being aligned and non-null to distinguish
///       them from other data. You can obtain a pointer that is usable as `data`
///       for zero-length slices using [`NonNull::dangling()`].
///
/// * `data` must point to `len` consecutive properly initialized values of type `T`.
///
/// * The memory referenced by the returned slice must not be accessed through any other pointer
///   (not derived from the return value) for the duration of lifetime `'a`.
///   Both read and write accesses are forbidden.
///
/// * The total size `len * mem::size_of::<T>()` of the slice must be no larger than `isize::MAX`.
///   See the safety documentation of [`pointer::offset`].
///
/// [valid]: ptr#safety
/// [`NonNull::dangling()`]: ptr::NonNull::dangling
/// [`pointer::offset`]: ../../std/primitive.pointer.html#method.offset
#[inline]
#[stable(feature = "rust1", since = "1.0.0")]
pub unsafe fn from_raw_parts_mut<'a, T>(data: *mut T, len: usize) -> &'a mut [T] {
    debug_assert!(is_aligned_and_not_null(data), "attempt to create unaligned or null slice");
    debug_assert!(
        mem::size_of::<T>().saturating_mul(len) <= isize::MAX as usize,
        "attempt to create slice covering at least half the address space"
    );
    // SAFETY: the caller must uphold the safety contract for `from_raw_parts_mut`.
    unsafe { &mut *ptr::slice_from_raw_parts_mut(data, len) }
}

/// Converts a reference to T into a slice of length 1 (without copying).
#[stable(feature = "from_ref", since = "1.28.0")]
pub fn from_ref<T>(s: &T) -> &[T] {
    array::from_ref(s)
}

/// Converts a reference to T into a slice of length 1 (without copying).
#[stable(feature = "from_ref", since = "1.28.0")]
pub fn from_mut<T>(s: &mut T) -> &mut [T] {
    array::from_mut(s)
}
