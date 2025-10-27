// ignore-tidy-filelength

//! Slice management and manipulation.
//!
//! For more details see [`std::slice`].
//!
//! [`std::slice`]: ../../std/slice/index.html

#![stable(feature = "rust1", since = "1.0.0")]

use crate::cmp::Ordering::{self, Equal, Greater, Less};
use crate::marker::Copy;
use crate::mem;
use crate::num::NonZeroUsize;
use crate::ops::{FnMut, Range, RangeBounds};
use crate::option::Option;
use crate::option::Option::{None, Some};
use crate::ptr;
use crate::result::Result;
use crate::result::Result::{Err, Ok};

#[unstable(
    feature = "slice_internals",
    issue = "none",
    reason = "exposed from core to be reused in std; use the memchr crate"
)]
/// Pure rust memchr implementation, taken from rust-memchr
pub mod memchr;

mod ascii;
mod cmp;
pub(crate) mod index;
mod iter;
mod raw;
mod rotate;
mod sort;

#[stable(feature = "rust1", since = "1.0.0")]
pub use iter::{Chunks, ChunksMut, Windows};
#[stable(feature = "rust1", since = "1.0.0")]
pub use iter::{Iter, IterMut};
#[stable(feature = "rust1", since = "1.0.0")]
pub use iter::{RSplitN, RSplitNMut, Split, SplitMut, SplitN, SplitNMut};

#[stable(feature = "slice_rsplit", since = "1.27.0")]
pub use iter::{RSplit, RSplitMut};

#[stable(feature = "chunks_exact", since = "1.31.0")]
pub use iter::{ChunksExact, ChunksExactMut};

#[stable(feature = "rchunks", since = "1.31.0")]
pub use iter::{RChunks, RChunksExact, RChunksExactMut, RChunksMut};

#[unstable(feature = "array_chunks", issue = "74985")]
pub use iter::{ArrayChunks, ArrayChunksMut};

#[unstable(feature = "array_windows", issue = "75027")]
pub use iter::ArrayWindows;

#[unstable(feature = "split_inclusive", issue = "72360")]
pub use iter::{SplitInclusive, SplitInclusiveMut};

#[stable(feature = "rust1", since = "1.0.0")]
pub use raw::{from_raw_parts, from_raw_parts_mut};

#[stable(feature = "from_ref", since = "1.28.0")]
pub use raw::{from_mut, from_ref};

// This function is public only because there is no other way to unit test heapsort.
#[unstable(feature = "sort_internals", reason = "internal to sort module", issue = "none")]
pub use sort::heapsort;

#[stable(feature = "slice_get_slice", since = "1.28.0")]
pub use index::SliceIndex;

#[lang = "slice"]
#[cfg(not(test))]
impl<T> [T] {
    /// Returns the number of elements in the slice.
    ///
    /// # Examples
    ///
    /// ```
    /// let a = [1, 2, 3];
    /// assert_eq!(a.len(), 3);
    /// ```
    #[stable(feature = "rust1", since = "1.0.0")]
    #[rustc_const_stable(feature = "const_slice_len", since = "1.32.0")]
    #[inline]
    // SAFETY: const sound because we transmute out the length field as a usize (which it must be)
    #[cfg_attr(not(bootstrap), rustc_allow_const_fn_unstable(const_fn_union))]
    #[cfg_attr(bootstrap, allow_internal_unstable(const_fn_union))]
    pub const fn len(&self) -> usize {
        // SAFETY: this is safe because `&[T]` and `FatPtr<T>` have the same layout.
        // Only `std` can make this guarantee.
        unsafe { crate::ptr::Repr { rust: self }.raw.len }
    }

    /// Returns `true` if the slice has a length of 0.
    ///
    /// # Examples
    ///
    /// ```
    /// let a = [1, 2, 3];
    /// assert!(!a.is_empty());
    /// ```
    #[stable(feature = "rust1", since = "1.0.0")]
    #[rustc_const_stable(feature = "const_slice_is_empty", since = "1.32.0")]
    #[inline]
    pub const fn is_empty(&self) -> bool {
        self.len() == 0
    }

    /// Returns the first element of the slice, or `None` if it is empty.
    ///
    /// # Examples
    ///
    /// ```
    /// let v = [10, 40, 30];
    /// assert_eq!(Some(&10), v.first());
    ///
    /// let w: &[i32] = &[];
    /// assert_eq!(None, w.first());
    /// ```
    #[stable(feature = "rust1", since = "1.0.0")]
    #[inline]
    pub fn first(&self) -> Option<&T> {
        if let [first, ..] = self { Some(first) } else { None }
    }

    /// Returns a mutable pointer to the first element of the slice, or `None` if it is empty.
    ///
    /// # Examples
    ///
    /// ```
    /// let x = &mut [0, 1, 2];
    ///
    /// if let Some(first) = x.first_mut() {
    ///     *first = 5;
    /// }
    /// assert_eq!(x, &[5, 1, 2]);
    /// ```
    #[stable(feature = "rust1", since = "1.0.0")]
    #[inline]
    pub fn first_mut(&mut self) -> Option<&mut T> {
        if let [first, ..] = self { Some(first) } else { None }
    }

    /// Returns the first and all the rest of the elements of the slice, or `None` if it is empty.
    ///
    /// # Examples
    ///
    /// ```
    /// let x = &[0, 1, 2];
    ///
    /// if let Some((first, elements)) = x.split_first() {
    ///     assert_eq!(first, &0);
    ///     assert_eq!(elements, &[1, 2]);
    /// }
    /// ```
    #[stable(feature = "slice_splits", since = "1.5.0")]
    #[inline]
    pub fn split_first(&self) -> Option<(&T, &[T])> {
        if let [first, tail @ ..] = self { Some((first, tail)) } else { None }
    }

    /// Returns the first and all the rest of the elements of the slice, or `None` if it is empty.
    ///
    /// # Examples
    ///
    /// ```
    /// let x = &mut [0, 1, 2];
    ///
    /// if let Some((first, elements)) = x.split_first_mut() {
    ///     *first = 3;
    ///     elements[0] = 4;
    ///     elements[1] = 5;
    /// }
    /// assert_eq!(x, &[3, 4, 5]);
    /// ```
    #[stable(feature = "slice_splits", since = "1.5.0")]
    #[inline]
    pub fn split_first_mut(&mut self) -> Option<(&mut T, &mut [T])> {
        if let [first, tail @ ..] = self { Some((first, tail)) } else { None }
    }

    /// Returns the last and all the rest of the elements of the slice, or `None` if it is empty.
    ///
    /// # Examples
    ///
    /// ```
    /// let x = &[0, 1, 2];
    ///
    /// if let Some((last, elements)) = x.split_last() {
    ///     assert_eq!(last, &2);
    ///     assert_eq!(elements, &[0, 1]);
    /// }
    /// ```
    #[stable(feature = "slice_splits", since = "1.5.0")]
    #[inline]
    pub fn split_last(&self) -> Option<(&T, &[T])> {
        if let [init @ .., last] = self { Some((last, init)) } else { None }
    }

    /// Returns the last and all the rest of the elements of the slice, or `None` if it is empty.
    ///
    /// # Examples
    ///
    /// ```
    /// let x = &mut [0, 1, 2];
    ///
    /// if let Some((last, elements)) = x.split_last_mut() {
    ///     *last = 3;
    ///     elements[0] = 4;
    ///     elements[1] = 5;
    /// }
    /// assert_eq!(x, &[4, 5, 3]);
    /// ```
    #[stable(feature = "slice_splits", since = "1.5.0")]
    #[inline]
    pub fn split_last_mut(&mut self) -> Option<(&mut T, &mut [T])> {
        if let [init @ .., last] = self { Some((last, init)) } else { None }
    }

    /// Returns the last element of the slice, or `None` if it is empty.
    ///
    /// # Examples
    ///
    /// ```
    /// let v = [10, 40, 30];
    /// assert_eq!(Some(&30), v.last());
    ///
    /// let w: &[i32] = &[];
    /// assert_eq!(None, w.last());
    /// ```
    #[stable(feature = "rust1", since = "1.0.0")]
    #[inline]
    pub fn last(&self) -> Option<&T> {
        if let [.., last] = self { Some(last) } else { None }
    }

    /// Returns a mutable pointer to the last item in the slice.
    ///
    /// # Examples
    ///
    /// ```
    /// let x = &mut [0, 1, 2];
    ///
    /// if let Some(last) = x.last_mut() {
    ///     *last = 10;
    /// }
    /// assert_eq!(x, &[0, 1, 10]);
    /// ```
    #[stable(feature = "rust1", since = "1.0.0")]
    #[inline]
    pub fn last_mut(&mut self) -> Option<&mut T> {
        if let [.., last] = self { Some(last) } else { None }
    }

    /// Returns a reference to an element or subslice depending on the type of
    /// index.
    ///
    /// - If given a position, returns a reference to the element at that
    ///   position or `None` if out of bounds.
    /// - If given a range, returns the subslice corresponding to that range,
    ///   or `None` if out of bounds.
    ///
    /// # Examples
    ///
    /// ```
    /// let v = [10, 40, 30];
    /// assert_eq!(Some(&40), v.get(1));
    /// assert_eq!(Some(&[10, 40][..]), v.get(0..2));
    /// assert_eq!(None, v.get(3));
    /// assert_eq!(None, v.get(0..4));
    /// ```
    #[stable(feature = "rust1", since = "1.0.0")]
    #[inline]
    pub fn get<I>(&self, index: I) -> Option<&I::Output>
    where
        I: SliceIndex<Self>,
    {
        index.get(self)
    }

    /// Returns a mutable reference to an element or subslice depending on the
    /// type of index (see [`get`]) or `None` if the index is out of bounds.
    ///
    /// [`get`]: #method.get
    ///
    /// # Examples
    ///
    /// ```
    /// let x = &mut [0, 1, 2];
    ///
    /// if let Some(elem) = x.get_mut(1) {
    ///     *elem = 42;
    /// }
    /// assert_eq!(x, &[0, 42, 2]);
    /// ```
    #[stable(feature = "rust1", since = "1.0.0")]
    #[inline]
    pub fn get_mut<I>(&mut self, index: I) -> Option<&mut I::Output>
    where
        I: SliceIndex<Self>,
    {
        index.get_mut(self)
    }

    /// Returns a reference to an element or subslice, without doing bounds
    /// checking.
    ///
    /// For a safe alternative see [`get`].
    ///
    /// # Safety
    ///
    /// Calling this method with an out-of-bounds index is *[undefined behavior]*
    /// even if the resulting reference is not used.
    ///
    /// [`get`]: #method.get
    /// [undefined behavior]: https://doc.rust-lang.org/reference/behavior-considered-undefined.html
    ///
    /// # Examples
    ///
    /// ```
    /// let x = &[1, 2, 4];
    ///
    /// unsafe {
    ///     assert_eq!(x.get_unchecked(1), &2);
    /// }
    /// ```
    #[stable(feature = "rust1", since = "1.0.0")]
    #[inline]
    pub unsafe fn get_unchecked<I>(&self, index: I) -> &I::Output
    where
        I: SliceIndex<Self>,
    {
        // SAFETY: the caller must uphold most of the safety requirements for `get_unchecked`;
        // the slice is dereferencable because `self` is a safe reference.
        // The returned pointer is safe because impls of `SliceIndex` have to guarantee that it is.
        unsafe { &*index.get_unchecked(self) }
    }

    /// Returns a mutable reference to an element or subslice, without doing
    /// bounds checking.
    ///
    /// For a safe alternative see [`get_mut`].
    ///
    /// # Safety
    ///
    /// Calling this method with an out-of-bounds index is *[undefined behavior]*
    /// even if the resulting reference is not used.
    ///
    /// [`get_mut`]: #method.get_mut
    /// [undefined behavior]: https://doc.rust-lang.org/reference/behavior-considered-undefined.html
    ///
    /// # Examples
    ///
    /// ```
    /// let x = &mut [1, 2, 4];
    ///
    /// unsafe {
    ///     let elem = x.get_unchecked_mut(1);
    ///     *elem = 13;
    /// }
    /// assert_eq!(x, &[1, 13, 4]);
    /// ```
    #[stable(feature = "rust1", since = "1.0.0")]
    #[inline]
    pub unsafe fn get_unchecked_mut<I>(&mut self, index: I) -> &mut I::Output
    where
        I: SliceIndex<Self>,
    {
        // SAFETY: the caller must uphold the safety requirements for `get_unchecked_mut`;
        // the slice is dereferencable because `self` is a safe reference.
        // The returned pointer is safe because impls of `SliceIndex` have to guarantee that it is.
        unsafe { &mut *index.get_unchecked_mut(self) }
    }

    /// Returns a raw pointer to the slice's buffer.
    ///
    /// The caller must ensure that the slice outlives the pointer this
    /// function returns, or else it will end up pointing to garbage.
    ///
    /// The caller must also ensure that the memory the pointer (non-transitively) points to
    /// is never written to (except inside an `UnsafeCell`) using this pointer or any pointer
    /// derived from it. If you need to mutate the contents of the slice, use [`as_mut_ptr`].
    ///
    /// Modifying the container referenced by this slice may cause its buffer
    /// to be reallocated, which would also make any pointers to it invalid.
    ///
    /// # Examples
    ///
    /// ```
    /// let x = &[1, 2, 4];
    /// let x_ptr = x.as_ptr();
    ///
    /// unsafe {
    ///     for i in 0..x.len() {
    ///         assert_eq!(x.get_unchecked(i), &*x_ptr.add(i));
    ///     }
    /// }
    /// ```
    ///
    /// [`as_mut_ptr`]: #method.as_mut_ptr
    #[stable(feature = "rust1", since = "1.0.0")]
    #[rustc_const_stable(feature = "const_slice_as_ptr", since = "1.32.0")]
    #[inline]
    pub const fn as_ptr(&self) -> *const T {
        self as *const [T] as *const T
    }

    /// Returns an unsafe mutable pointer to the slice's buffer.
    ///
    /// The caller must ensure that the slice outlives the pointer this
    /// function returns, or else it will end up pointing to garbage.
    ///
    /// Modifying the container referenced by this slice may cause its buffer
    /// to be reallocated, which would also make any pointers to it invalid.
    ///
    /// # Examples
    ///
    /// ```
    /// let x = &mut [1, 2, 4];
    /// let x_ptr = x.as_mut_ptr();
    ///
    /// unsafe {
    ///     for i in 0..x.len() {
    ///         *x_ptr.add(i) += 2;
    ///     }
    /// }
    /// assert_eq!(x, &[3, 4, 6]);
    /// ```
    #[stable(feature = "rust1", since = "1.0.0")]
    #[rustc_const_unstable(feature = "const_ptr_offset", issue = "71499")]
    #[inline]
    pub const fn as_mut_ptr(&mut self) -> *mut T {
        self as *mut [T] as *mut T
    }

    /// Returns the two raw pointers spanning the slice.
    ///
    /// The returned range is half-open, which means that the end pointer
    /// points *one past* the last element of the slice. This way, an empty
    /// slice is represented by two equal pointers, and the difference between
    /// the two pointers represents the size of the slice.
    ///
    /// See [`as_ptr`] for warnings on using these pointers. The end pointer
    /// requires extra caution, as it does not point to a valid element in the
    /// slice.
    ///
    /// This function is useful for interacting with foreign interfaces which
    /// use two pointers to refer to a range of elements in memory, as is
    /// common in C++.
    ///
    /// It can also be useful to check if a pointer to an element refers to an
    /// element of this slice:
    ///
    /// ```
    /// let a = [1, 2, 3];
    /// let x = &a[1] as *const _;
    /// let y = &5 as *const _;
    ///
    /// assert!(a.as_ptr_range().contains(&x));
    /// assert!(!a.as_ptr_range().contains(&y));
    /// ```
    ///
    /// [`as_ptr`]: #method.as_ptr
    #[stable(feature = "slice_ptr_range", since = "1.48.0")]
    #[rustc_const_unstable(feature = "const_ptr_offset", issue = "71499")]
    #[inline]
    pub const fn as_ptr_range(&self) -> Range<*const T> {
        let start = self.as_ptr();
        // SAFETY: The `add` here is safe, because:
        //
        //   - Both pointers are part of the same object, as pointing directly
        //     past the object also counts.
        //
        //   - The size of the slice is never larger than isize::MAX bytes, as
        //     noted here:
        //       - https://github.com/rust-lang/unsafe-code-guidelines/issues/102#issuecomment-473340447
        //       - https://doc.rust-lang.org/reference/behavior-considered-undefined.html
        //       - https://doc.rust-lang.org/core/slice/fn.from_raw_parts.html#safety
        //     (This doesn't seem normative yet, but the very same assumption is
        //     made in many places, including the Index implementation of slices.)
        //
        //   - There is no wrapping around involved, as slices do not wrap past
        //     the end of the address space.
        //
        // See the documentation of pointer::add.
        let end = unsafe { start.add(self.len()) };
        start..end
    }

    /// Returns the two unsafe mutable pointers spanning the slice.
    ///
    /// The returned range is half-open, which means that the end pointer
    /// points *one past* the last element of the slice. This way, an empty
    /// slice is represented by two equal pointers, and the difference between
    /// the two pointers represents the size of the slice.
    ///
    /// See [`as_mut_ptr`] for warnings on using these pointers. The end
    /// pointer requires extra caution, as it does not point to a valid element
    /// in the slice.
    ///
    /// This function is useful for interacting with foreign interfaces which
    /// use two pointers to refer to a range of elements in memory, as is
    /// common in C++.
    ///
    /// [`as_mut_ptr`]: #method.as_mut_ptr
    #[stable(feature = "slice_ptr_range", since = "1.48.0")]
    #[rustc_const_unstable(feature = "const_ptr_offset", issue = "71499")]
    #[inline]
    pub const fn as_mut_ptr_range(&mut self) -> Range<*mut T> {
        let start = self.as_mut_ptr();
        // SAFETY: See as_ptr_range() above for why `add` here is safe.
        let end = unsafe { start.add(self.len()) };
        start..end
    }

    /// Swaps two elements in the slice.
    ///
    /// # Arguments
    ///
    /// * a - The index of the first element
    /// * b - The index of the second element
    ///
    /// # Panics
    ///
    /// Panics if `a` or `b` are out of bounds.
    ///
    /// # Examples
    ///
    /// ```
    /// let mut v = ["a", "b", "c", "d"];
    /// v.swap(1, 3);
    /// assert!(v == ["a", "d", "c", "b"]);
    /// ```
    #[stable(feature = "rust1", since = "1.0.0")]
    #[inline]
    pub fn swap(&mut self, a: usize, b: usize) {
        // Can't take two mutable loans from one vector, so instead just cast
        // them to their raw pointers to do the swap.
        let pa: *mut T = &mut self[a];
        let pb: *mut T = &mut self[b];
        // SAFETY: `pa` and `pb` have been created from safe mutable references and refer
        // to elements in the slice and therefore are guaranteed to be valid and aligned.
        // Note that accessing the elements behind `a` and `b` is checked and will
        // panic when out of bounds.
        unsafe {
            ptr::swap(pa, pb);
        }
    }

    /// Reverses the order of elements in the slice, in place.
    ///
    /// # Examples
    ///
    /// ```
    /// let mut v = [1, 2, 3];
    /// v.reverse();
    /// assert!(v == [3, 2, 1]);
    /// ```
    #[stable(feature = "rust1", since = "1.0.0")]
    #[inline]
    pub fn reverse(&mut self) {
        let mut i: usize = 0;
        let ln = self.len();

        // For very small types, all the individual reads in the normal
        // path perform poorly.  We can do better, given efficient unaligned
        // load/store, by loading a larger chunk and reversing a register.

        // Ideally LLVM would do this for us, as it knows better than we do
        // whether unaligned reads are efficient (since that changes between
        // different ARM versions, for example) and what the best chunk size
        // would be.  Unfortunately, as of LLVM 4.0 (2017-05) it only unrolls
        // the loop, so we need to do this ourselves.  (Hypothesis: reverse
        // is troublesome because the sides can be aligned differently --
        // will be, when the length is odd -- so there's no way of emitting
        // pre- and postludes to use fully-aligned SIMD in the middle.)

        let fast_unaligned = cfg!(any(target_arch = "x86", target_arch = "x86_64"));

        if fast_unaligned && mem::size_of::<T>() == 1 {
            // Use the llvm.bswap intrinsic to reverse u8s in a usize
            let chunk = mem::size_of::<usize>();
            while i + chunk - 1 < ln / 2 {
                // SAFETY: There are several things to check here:
                //
                // - Note that `chunk` is either 4 or 8 due to the cfg check
                //   above. So `chunk - 1` is positive.
                // - Indexing with index `i` is fine as the loop check guarantees
                //   `i + chunk - 1 < ln / 2`
                //   <=> `i < ln / 2 - (chunk - 1) < ln / 2 < ln`.
                // - Indexing with index `ln - i - chunk = ln - (i + chunk)` is fine:
                //   - `i + chunk > 0` is trivially true.
                //   - The loop check guarantees:
                //     `i + chunk - 1 < ln / 2`
                //     <=> `i + chunk ≤ ln / 2 ≤ ln`, thus subtraction does not underflow.
                // - The `read_unaligned` and `write_unaligned` calls are fine:
                //   - `pa` points to index `i` where `i < ln / 2 - (chunk - 1)`
                //     (see above) and `pb` points to index `ln - i - chunk`, so
                //     both are at least `chunk`
                //     many bytes away from the end of `self`.
                //   - Any initialized memory is valid `usize`.
                unsafe {
                    let pa: *mut T = self.get_unchecked_mut(i);
                    let pb: *mut T = self.get_unchecked_mut(ln - i - chunk);
                    let va = ptr::read_unaligned(pa as *mut usize);
                    let vb = ptr::read_unaligned(pb as *mut usize);
                    ptr::write_unaligned(pa as *mut usize, vb.swap_bytes());
                    ptr::write_unaligned(pb as *mut usize, va.swap_bytes());
                }
                i += chunk;
            }
        }

        if fast_unaligned && mem::size_of::<T>() == 2 {
            // Use rotate-by-16 to reverse u16s in a u32
            let chunk = mem::size_of::<u32>() / 2;
            while i + chunk - 1 < ln / 2 {
                // SAFETY: An unaligned u32 can be read from `i` if `i + 1 < ln`
                // (and obviously `i < ln`), because each element is 2 bytes and
                // we're reading 4.
                //
                // `i + chunk - 1 < ln / 2` # while condition
                // `i + 2 - 1 < ln / 2`
                // `i + 1 < ln / 2`
                //
                // Since it's less than the length divided by 2, then it must be
                // in bounds.
                //
                // This also means that the condition `0 < i + chunk <= ln` is
                // always respected, ensuring the `pb` pointer can be used
                // safely.
                unsafe {
                    let pa: *mut T = self.get_unchecked_mut(i);
                    let pb: *mut T = self.get_unchecked_mut(ln - i - chunk);
                    let va = ptr::read_unaligned(pa as *mut u32);
                    let vb = ptr::read_unaligned(pb as *mut u32);
                    ptr::write_unaligned(pa as *mut u32, vb.rotate_left(16));
                    ptr::write_unaligned(pb as *mut u32, va.rotate_left(16));
                }
                i += chunk;
            }
        }

        while i < ln / 2 {
            // SAFETY: `i` is inferior to half the length of the slice so
            // accessing `i` and `ln - i - 1` is safe (`i` starts at 0 and
            // will not go further than `ln / 2 - 1`).
            // The resulting pointers `pa` and `pb` are therefore valid and
            // aligned, and can be read from and written to.
            unsafe {
                // Unsafe swap to avoid the bounds check in safe swap.
                let pa: *mut T = self.get_unchecked_mut(i);
                let pb: *mut T = self.get_unchecked_mut(ln - i - 1);
                ptr::swap(pa, pb);
            }
            i += 1;
        }
    }

    /// Returns an iterator over the slice.
    ///
    /// # Examples
    ///
    /// ```
    /// let x = &[1, 2, 4];
    /// let mut iterator = x.iter();
    ///
    /// assert_eq!(iterator.next(), Some(&1));
    /// assert_eq!(iterator.next(), Some(&2));
    /// assert_eq!(iterator.next(), Some(&4));
    /// assert_eq!(iterator.next(), None);
    /// ```
    #[stable(feature = "rust1", since = "1.0.0")]
    #[inline]
    pub fn iter(&self) -> Iter<'_, T> {
        Iter::new(self)
    }

    /// Returns an iterator that allows modifying each value.
    ///
    /// # Examples
    ///
    /// ```
    /// let x = &mut [1, 2, 4];
    /// for elem in x.iter_mut() {
    ///     *elem += 2;
    /// }
    /// assert_eq!(x, &[3, 4, 6]);
    /// ```
    #[stable(feature = "rust1", since = "1.0.0")]
    #[inline]
    pub fn iter_mut(&mut self) -> IterMut<'_, T> {
        IterMut::new(self)
    }

    /// Returns an iterator over all contiguous windows of length
    /// `size`. The windows overlap. If the slice is shorter than
    /// `size`, the iterator returns no values.
    ///
    /// # Panics
    ///
    /// Panics if `size` is 0.
    ///
    /// # Examples
    ///
    /// ```
    /// let slice = ['r', 'u', 's', 't'];
    /// let mut iter = slice.windows(2);
    /// assert_eq!(iter.next().unwrap(), &['r', 'u']);
    /// assert_eq!(iter.next().unwrap(), &['u', 's']);
    /// assert_eq!(iter.next().unwrap(), &['s', 't']);
    /// assert!(iter.next().is_none());
    /// ```
    ///
    /// If the slice is shorter than `size`:
    ///
    /// ```
    /// let slice = ['f', 'o', 'o'];
    /// let mut iter = slice.windows(4);
    /// assert!(iter.next().is_none());
    /// ```
    #[stable(feature = "rust1", since = "1.0.0")]
    #[inline]
    pub fn windows(&self, size: usize) -> Windows<'_, T> {
        let size = NonZeroUsize::new(size).expect("size is zero");
        Windows::new(self, size)
    }

    /// Returns an iterator over `chunk_size` elements of the slice at a time, starting at the
    /// beginning of the slice.
    ///
    /// The chunks are slices and do not overlap. If `chunk_size` does not divide the length of the
    /// slice, then the last chunk will not have length `chunk_size`.
    ///
    /// See [`chunks_exact`] for a variant of this iterator that returns chunks of always exactly
    /// `chunk_size` elements, and [`rchunks`] for the same iterator but starting at the end of the
    /// slice.
    ///
    /// # Panics
    ///
    /// Panics if `chunk_size` is 0.
    ///
    /// # Examples
    ///
    /// ```
    /// let slice = ['l', 'o', 'r', 'e', 'm'];
    /// let mut iter = slice.chunks(2);
    /// assert_eq!(iter.next().unwrap(), &['l', 'o']);
    /// assert_eq!(iter.next().unwrap(), &['r', 'e']);
    /// assert_eq!(iter.next().unwrap(), &['m']);
    /// assert!(iter.next().is_none());
    /// ```
    ///
    /// [`chunks_exact`]: #method.chunks_exact
    /// [`rchunks`]: #method.rchunks
    #[stable(feature = "rust1", since = "1.0.0")]
    #[inline]
    pub fn chunks(&self, chunk_size: usize) -> Chunks<'_, T> {
        assert_ne!(chunk_size, 0);
        Chunks::new(self, chunk_size)
    }

    /// Returns an iterator over `chunk_size` elements of the slice at a time, starting at the
    /// beginning of the slice.
    ///
    /// The chunks are mutable slices, and do not overlap. If `chunk_size` does not divide the
    /// length of the slice, then the last chunk will not have length `chunk_size`.
    ///
    /// See [`chunks_exact_mut`] for a variant of this iterator that returns chunks of always
    /// exactly `chunk_size` elements, and [`rchunks_mut`] for the same iterator but starting at
    /// the end of the slice.
    ///
    /// # Panics
    ///
    /// Panics if `chunk_size` is 0.
    ///
    /// # Examples
    ///
    /// ```
    /// let v = &mut [0, 0, 0, 0, 0];
    /// let mut count = 1;
    ///
    /// for chunk in v.chunks_mut(2) {
    ///     for elem in chunk.iter_mut() {
    ///         *elem += count;
    ///     }
    ///     count += 1;
    /// }
    /// assert_eq!(v, &[1, 1, 2, 2, 3]);
    /// ```
    ///
    /// [`chunks_exact_mut`]: #method.chunks_exact_mut
    /// [`rchunks_mut`]: #method.rchunks_mut
    #[stable(feature = "rust1", since = "1.0.0")]
    #[inline]
    pub fn chunks_mut(&mut self, chunk_size: usize) -> ChunksMut<'_, T> {
        assert_ne!(chunk_size, 0);
        ChunksMut::new(self, chunk_size)
    }

    /// Returns an iterator over `chunk_size` elements of the slice at a time, starting at the
    /// beginning of the slice.
    ///
    /// The chunks are slices and do not overlap. If `chunk_size` does not divide the length of the
    /// slice, then the last up to `chunk_size-1` elements will be omitted and can be retrieved
    /// from the `remainder` function of the iterator.
    ///
    /// Due to each chunk having exactly `chunk_size` elements, the compiler can often optimize the
    /// resulting code better than in the case of [`chunks`].
    ///
    /// See [`chunks`] for a variant of this iterator that also returns the remainder as a smaller
    /// chunk, and [`rchunks_exact`] for the same iterator but starting at the end of the slice.
    ///
    /// # Panics
    ///
    /// Panics if `chunk_size` is 0.
    ///
    /// # Examples
    ///
    /// ```
    /// let slice = ['l', 'o', 'r', 'e', 'm'];
    /// let mut iter = slice.chunks_exact(2);
    /// assert_eq!(iter.next().unwrap(), &['l', 'o']);
    /// assert_eq!(iter.next().unwrap(), &['r', 'e']);
    /// assert!(iter.next().is_none());
    /// assert_eq!(iter.remainder(), &['m']);
    /// ```
    ///
    /// [`chunks`]: #method.chunks
    /// [`rchunks_exact`]: #method.rchunks_exact
    #[stable(feature = "chunks_exact", since = "1.31.0")]
    #[inline]
    pub fn chunks_exact(&self, chunk_size: usize) -> ChunksExact<'_, T> {
        assert_ne!(chunk_size, 0);
        ChunksExact::new(self, chunk_size)
    }

    /// Returns an iterator over `chunk_size` elements of the slice at a time, starting at the
    /// beginning of the slice.
    ///
    /// The chunks are mutable slices, and do not overlap. If `chunk_size` does not divide the
    /// length of the slice, then the last up to `chunk_size-1` elements will be omitted and can be
    /// retrieved from the `into_remainder` function of the iterator.
    ///
    /// Due to each chunk having exactly `chunk_size` elements, the compiler can often optimize the
    /// resulting code better than in the case of [`chunks_mut`].
    ///
    /// See [`chunks_mut`] for a variant of this iterator that also returns the remainder as a
    /// smaller chunk, and [`rchunks_exact_mut`] for the same iterator but starting at the end of
    /// the slice.
    ///
    /// # Panics
    ///
    /// Panics if `chunk_size` is 0.
    ///
    /// # Examples
    ///
    /// ```
    /// let v = &mut [0, 0, 0, 0, 0];
    /// let mut count = 1;
    ///
    /// for chunk in v.chunks_exact_mut(2) {
    ///     for elem in chunk.iter_mut() {
    ///         *elem += count;
    ///     }
    ///     count += 1;
    /// }
    /// assert_eq!(v, &[1, 1, 2, 2, 0]);
    /// ```
    ///
    /// [`chunks_mut`]: #method.chunks_mut
    /// [`rchunks_exact_mut`]: #method.rchunks_exact_mut
    #[stable(feature = "chunks_exact", since = "1.31.0")]
    #[inline]
    pub fn chunks_exact_mut(&mut self, chunk_size: usize) -> ChunksExactMut<'_, T> {
        assert_ne!(chunk_size, 0);
        ChunksExactMut::new(self, chunk_size)
    }

    /// Splits the slice into a slice of `N`-element arrays,
    /// starting at the beginning of the slice,
    /// and a remainder slice with length strictly less than `N`.
    ///
    /// # Panics
    ///
    /// Panics if `N` is 0. This check will most probably get changed to a compile time
    /// error before this method gets stabilized.
    ///
    /// # Examples
    ///
    /// ```
    /// #![feature(slice_as_chunks)]
    /// let slice = ['l', 'o', 'r', 'e', 'm'];
    /// let (chunks, remainder) = slice.as_chunks();
    /// assert_eq!(chunks, &[['l', 'o'], ['r', 'e']]);
    /// assert_eq!(remainder, &['m']);
    /// ```
    #[unstable(feature = "slice_as_chunks", issue = "74985")]
    #[inline]
    pub fn as_chunks<const N: usize>(&self) -> (&[[T; N]], &[T]) {
        assert_ne!(N, 0);
        let len = self.len() / N;
        let (multiple_of_n, remainder) = self.split_at(len * N);
        // SAFETY: We cast a slice of `len * N` elements into
        // a slice of `len` many `N` elements chunks.
        let array_slice: &[[T; N]] = unsafe { from_raw_parts(multiple_of_n.as_ptr().cast(), len) };
        (array_slice, remainder)
    }

    /// Returns an iterator over `N` elements of the slice at a time, starting at the
    /// beginning of the slice.
    ///
    /// The chunks are array references and do not overlap. If `N` does not divide the
    /// length of the slice, then the last up to `N-1` elements will be omitted and can be
    /// retrieved from the `remainder` function of the iterator.
    ///
    /// This method is the const generic equivalent of [`chunks_exact`].
    ///
    /// # Panics
    ///
    /// Panics if `N` is 0. This check will most probably get changed to a compile time
    /// error before this method gets stabilized.
    ///
    /// # Examples
    ///
    /// ```
    /// #![feature(array_chunks)]
    /// let slice = ['l', 'o', 'r', 'e', 'm'];
    /// let mut iter = slice.array_chunks();
    /// assert_eq!(iter.next().unwrap(), &['l', 'o']);
    /// assert_eq!(iter.next().unwrap(), &['r', 'e']);
    /// assert!(iter.next().is_none());
    /// assert_eq!(iter.remainder(), &['m']);
    /// ```
    ///
    /// [`chunks_exact`]: #method.chunks_exact
    #[unstable(feature = "array_chunks", issue = "74985")]
    #[inline]
    pub fn array_chunks<const N: usize>(&self) -> ArrayChunks<'_, T, N> {
        assert_ne!(N, 0);
        ArrayChunks::new(self)
    }

    /// Splits the slice into a slice of `N`-element arrays,
    /// starting at the beginning of the slice,
    /// and a remainder slice with length strictly less than `N`.
    ///
    /// # Panics
    ///
    /// Panics if `N` is 0. This check will most probably get changed to a compile time
    /// error before this method gets stabilized.
    ///
    /// # Examples
    ///
    /// ```
    /// #![feature(slice_as_chunks)]
    /// let v = &mut [0, 0, 0, 0, 0];
    /// let mut count = 1;
    ///
    /// let (chunks, remainder) = v.as_chunks_mut();
    /// remainder[0] = 9;
    /// for chunk in chunks {
    ///     *chunk = [count; 2];
    ///     count += 1;
    /// }
    /// assert_eq!(v, &[1, 1, 2, 2, 9]);
    /// ```
    #[unstable(feature = "slice_as_chunks", issue = "74985")]
    #[inline]
    pub fn as_chunks_mut<const N: usize>(&mut self) -> (&mut [[T; N]], &mut [T]) {
        assert_ne!(N, 0);
        let len = self.len() / N;
        let (multiple_of_n, remainder) = self.split_at_mut(len * N);
        let array_slice: &mut [[T; N]] =
            // SAFETY: We cast a slice of `len * N` elements into
            // a slice of `len` many `N` elements chunks.
            unsafe { from_raw_parts_mut(multiple_of_n.as_mut_ptr().cast(), len) };
        (array_slice, remainder)
    }

    /// Returns an iterator over `N` elements of the slice at a time, starting at the
    /// beginning of the slice.
    ///
    /// The chunks are mutable array references and do not overlap. If `N` does not divide
    /// the length of the slice, then the last up to `N-1` elements will be omitted and
    /// can be retrieved from the `into_remainder` function of the iterator.
    ///
    /// This method is the const generic equivalent of [`chunks_exact_mut`].
    ///
    /// # Panics
    ///
    /// Panics if `N` is 0. This check will most probably get changed to a compile time
    /// error before this method gets stabilized.
    ///
    /// # Examples
    ///
    /// ```
    /// #![feature(array_chunks)]
    /// let v = &mut [0, 0, 0, 0, 0];
    /// let mut count = 1;
    ///
    /// for chunk in v.array_chunks_mut() {
    ///     *chunk = [count; 2];
    ///     count += 1;
    /// }
    /// assert_eq!(v, &[1, 1, 2, 2, 0]);
    /// ```
    ///
    /// [`chunks_exact_mut`]: #method.chunks_exact_mut
    #[unstable(feature = "array_chunks", issue = "74985")]
    #[inline]
    pub fn array_chunks_mut<const N: usize>(&mut self) -> ArrayChunksMut<'_, T, N> {
        assert_ne!(N, 0);
        ArrayChunksMut::new(self)
    }

    /// Returns an iterator over overlapping windows of `N` elements of  a slice,
    /// starting at the beginning of the slice.
    ///
    /// This is the const generic equivalent of [`windows`].
    ///
    /// If `N` is greater than the size of the slice, it will return no windows.
    ///
    /// # Panics
    ///
    /// Panics if `N` is 0. This check will most probably get changed to a compile time
    /// error before this method gets stabilized.
    ///
    /// # Examples
    ///
    /// ```
    /// #![feature(array_windows)]
    /// let slice = [0, 1, 2, 3];
    /// let mut iter = slice.array_windows();
    /// assert_eq!(iter.next().unwrap(), &[0, 1]);
    /// assert_eq!(iter.next().unwrap(), &[1, 2]);
    /// assert_eq!(iter.next().unwrap(), &[2, 3]);
    /// assert!(iter.next().is_none());
    /// ```
    ///
    /// [`windows`]: #method.windows
    #[unstable(feature = "array_windows", issue = "75027")]
    #[inline]
    pub fn array_windows<const N: usize>(&self) -> ArrayWindows<'_, T, N> {
        assert_ne!(N, 0);
        ArrayWindows::new(self)
    }

    /// Returns an iterator over `chunk_size` elements of the slice at a time, starting at the end
    /// of the slice.
    ///
    /// The chunks are slices and do not overlap. If `chunk_size` does not divide the length of the
    /// slice, then the last chunk will not have length `chunk_size`.
    ///
    /// See [`rchunks_exact`] for a variant of this iterator that returns chunks of always exactly
    /// `chunk_size` elements, and [`chunks`] for the same iterator but starting at the beginning
    /// of the slice.
    ///
    /// # Panics
    ///
    /// Panics if `chunk_size` is 0.
    ///
    /// # Examples
    ///
    /// ```
    /// let slice = ['l', 'o', 'r', 'e', 'm'];
    /// let mut iter = slice.rchunks(2);
    /// assert_eq!(iter.next().unwrap(), &['e', 'm']);
    /// assert_eq!(iter.next().unwrap(), &['o', 'r']);
    /// assert_eq!(iter.next().unwrap(), &['l']);
    /// assert!(iter.next().is_none());
    /// ```
    ///
    /// [`rchunks_exact`]: #method.rchunks_exact
    /// [`chunks`]: #method.chunks
    #[stable(feature = "rchunks", since = "1.31.0")]
    #[inline]
    pub fn rchunks(&self, chunk_size: usize) -> RChunks<'_, T> {
        assert!(chunk_size != 0);
        RChunks::new(self, chunk_size)
    }

    /// Returns an iterator over `chunk_size` elements of the slice at a time, starting at the end
    /// of the slice.
    ///
    /// The chunks are mutable slices, and do not overlap. If `chunk_size` does not divide the
    /// length of the slice, then the last chunk will not have length `chunk_size`.
    ///
    /// See [`rchunks_exact_mut`] for a variant of this iterator that returns chunks of always
    /// exactly `chunk_size` elements, and [`chunks_mut`] for the same iterator but starting at the
    /// beginning of the slice.
    ///
    /// # Panics
    ///
    /// Panics if `chunk_size` is 0.
    ///
    /// # Examples
    ///
    /// ```
    /// let v = &mut [0, 0, 0, 0, 0];
    /// let mut count = 1;
    ///
    /// for chunk in v.rchunks_mut(2) {
    ///     for elem in chunk.iter_mut() {
    ///         *elem += count;
    ///     }
    ///     count += 1;
    /// }
    /// assert_eq!(v, &[3, 2, 2, 1, 1]);
    /// ```
    ///
    /// [`rchunks_exact_mut`]: #method.rchunks_exact_mut
    /// [`chunks_mut`]: #method.chunks_mut
    #[stable(feature = "rchunks", since = "1.31.0")]
    #[inline]
    pub fn rchunks_mut(&mut self, chunk_size: usize) -> RChunksMut<'_, T> {
        assert!(chunk_size != 0);
        RChunksMut::new(self, chunk_size)
    }

    /// Returns an iterator over `chunk_size` elements of the slice at a time, starting at the
    /// end of the slice.
    ///
    /// The chunks are slices and do not overlap. If `chunk_size` does not divide the length of the
    /// slice, then the last up to `chunk_size-1` elements will be omitted and can be retrieved
    /// from the `remainder` function of the iterator.
    ///
    /// Due to each chunk having exactly `chunk_size` elements, the compiler can often optimize the
    /// resulting code better than in the case of [`chunks`].
    ///
    /// See [`rchunks`] for a variant of this iterator that also returns the remainder as a smaller
    /// chunk, and [`chunks_exact`] for the same iterator but starting at the beginning of the
    /// slice.
    ///
    /// # Panics
    ///
    /// Panics if `chunk_size` is 0.
    ///
    /// # Examples
    ///
    /// ```
    /// let slice = ['l', 'o', 'r', 'e', 'm'];
    /// let mut iter = slice.rchunks_exact(2);
    /// assert_eq!(iter.next().unwrap(), &['e', 'm']);
    /// assert_eq!(iter.next().unwrap(), &['o', 'r']);
    /// assert!(iter.next().is_none());
    /// assert_eq!(iter.remainder(), &['l']);
    /// ```
    ///
    /// [`chunks`]: #method.chunks
    /// [`rchunks`]: #method.rchunks
    /// [`chunks_exact`]: #method.chunks_exact
    #[stable(feature = "rchunks", since = "1.31.0")]
    #[inline]
    pub fn rchunks_exact(&self, chunk_size: usize) -> RChunksExact<'_, T> {
        assert!(chunk_size != 0);
        RChunksExact::new(self, chunk_size)
    }

    /// Returns an iterator over `chunk_size` elements of the slice at a time, starting at the end
    /// of the slice.
    ///
    /// The chunks are mutable slices, and do not overlap. If `chunk_size` does not divide the
    /// length of the slice, then the last up to `chunk_size-1` elements will be omitted and can be
    /// retrieved from the `into_remainder` function of the iterator.
    ///
    /// Due to each chunk having exactly `chunk_size` elements, the compiler can often optimize the
    /// resulting code better than in the case of [`chunks_mut`].
    ///
    /// See [`rchunks_mut`] for a variant of this iterator that also returns the remainder as a
    /// smaller chunk, and [`chunks_exact_mut`] for the same iterator but starting at the beginning
    /// of the slice.
    ///
    /// # Panics
    ///
    /// Panics if `chunk_size` is 0.
    ///
    /// # Examples
    ///
    /// ```
    /// let v = &mut [0, 0, 0, 0, 0];
    /// let mut count = 1;
    ///
    /// for chunk in v.rchunks_exact_mut(2) {
    ///     for elem in chunk.iter_mut() {
    ///         *elem += count;
    ///     }
    ///     count += 1;
    /// }
    /// assert_eq!(v, &[0, 2, 2, 1, 1]);
    /// ```
    ///
    /// [`chunks_mut`]: #method.chunks_mut
    /// [`rchunks_mut`]: #method.rchunks_mut
    /// [`chunks_exact_mut`]: #method.chunks_exact_mut
    #[stable(feature = "rchunks", since = "1.31.0")]
    #[inline]
    pub fn rchunks_exact_mut(&mut self, chunk_size: usize) -> RChunksExactMut<'_, T> {
        assert!(chunk_size != 0);
        RChunksExactMut::new(self, chunk_size)
    }

    /// Divides one slice into two at an index.
    ///
    /// The first will contain all indices from `[0, mid)` (excluding
    /// the index `mid` itself) and the second will contain all
    /// indices from `[mid, len)` (excluding the index `len` itself).
    ///
    /// # Panics
    ///
    /// Panics if `mid > len`.
    ///
    /// # Examples
    ///
    /// ```
    /// let v = [1, 2, 3, 4, 5, 6];
    ///
    /// {
    ///    let (left, right) = v.split_at(0);
    ///    assert_eq!(left, []);
    ///    assert_eq!(right, [1, 2, 3, 4, 5, 6]);
    /// }
    ///
    /// {
    ///     let (left, right) = v.split_at(2);
    ///     assert_eq!(left, [1, 2]);
    ///     assert_eq!(right, [3, 4, 5, 6]);
    /// }
    ///
    /// {
    ///     let (left, right) = v.split_at(6);
    ///     assert_eq!(left, [1, 2, 3, 4, 5, 6]);
    ///     assert_eq!(right, []);
    /// }
    /// ```
    #[stable(feature = "rust1", since = "1.0.0")]
    #[inline]
    pub fn split_at(&self, mid: usize) -> (&[T], &[T]) {
        assert!(mid <= self.len());
        // SAFETY: `[ptr; mid]` and `[mid; len]` are inside `self`, which
        // fulfills the requirements of `from_raw_parts_mut`.
        unsafe { self.split_at_unchecked(mid) }
    }

    /// Divides one mutable slice into two at an index.
    ///
    /// The first will contain all indices from `[0, mid)` (excluding
    /// the index `mid` itself) and the second will contain all
    /// indices from `[mid, len)` (excluding the index `len` itself).
    ///
    /// # Panics
    ///
    /// Panics if `mid > len`.
    ///
    /// # Examples
    ///
    /// ```
    /// let mut v = [1, 0, 3, 0, 5, 6];
    /// // scoped to restrict the lifetime of the borrows
    /// {
    ///     let (left, right) = v.split_at_mut(2);
    ///     assert_eq!(left, [1, 0]);
    ///     assert_eq!(right, [3, 0, 5, 6]);
    ///     left[1] = 2;
    ///     right[1] = 4;
    /// }
    /// assert_eq!(v, [1, 2, 3, 4, 5, 6]);
    /// ```
    #[stable(feature = "rust1", since = "1.0.0")]
    #[inline]
    pub fn split_at_mut(&mut self, mid: usize) -> (&mut [T], &mut [T]) {
        assert!(mid <= self.len());
        // SAFETY: `[ptr; mid]` and `[mid; len]` are inside `self`, which
        // fulfills the requirements of `from_raw_parts_mut`.
        unsafe { self.split_at_mut_unchecked(mid) }
    }

    /// Divides one slice into two at an index, without doing bounds checking.
    ///
    /// The first will contain all indices from `[0, mid)` (excluding
    /// the index `mid` itself) and the second will contain all
    /// indices from `[mid, len)` (excluding the index `len` itself).
    ///
    /// For a safe alternative see [`split_at`].
    ///
    /// # Safety
    ///
    /// Calling this method with an out-of-bounds index is *[undefined behavior]*
    /// even if the resulting reference is not used. The caller has to ensure that
    /// `0 <= mid <= self.len()`.
    ///
    /// [`split_at`]: #method.split_at
    /// [undefined behavior]: https://doc.rust-lang.org/reference/behavior-considered-undefined.html
    ///
    /// # Examples
    ///
    /// ```compile_fail
    /// #![feature(slice_split_at_unchecked)]
    ///
    /// let v = [1, 2, 3, 4, 5, 6];
    ///
    /// unsafe {
    ///    let (left, right) = v.split_at_unchecked(0);
    ///    assert_eq!(left, []);
    ///    assert_eq!(right, [1, 2, 3, 4, 5, 6]);
    /// }
    ///
    /// unsafe {
    ///     let (left, right) = v.split_at_unchecked(2);
    ///     assert_eq!(left, [1, 2]);
    ///     assert_eq!(right, [3, 4, 5, 6]);
    /// }
    ///
    /// unsafe {
    ///     let (left, right) = v.split_at_unchecked(6);
    ///     assert_eq!(left, [1, 2, 3, 4, 5, 6]);
    ///     assert_eq!(right, []);
    /// }
    /// ```
    #[unstable(feature = "slice_split_at_unchecked", reason = "new API", issue = "76014")]
    #[inline]
    unsafe fn split_at_unchecked(&self, mid: usize) -> (&[T], &[T]) {
        // SAFETY: Caller has to check that `0 <= mid <= self.len()`
        unsafe { (self.get_unchecked(..mid), self.get_unchecked(mid..)) }
    }

    /// Divides one mutable slice into two at an index, without doing bounds checking.
    ///
    /// The first will contain all indices from `[0, mid)` (excluding
    /// the index `mid` itself) and the second will contain all
    /// indices from `[mid, len)` (excluding the index `len` itself).
    ///
    /// For a safe alternative see [`split_at_mut`].
    ///
    /// # Safety
    ///
    /// Calling this method with an out-of-bounds index is *[undefined behavior]*
    /// even if the resulting reference is not used. The caller has to ensure that
    /// `0 <= mid <= self.len()`.
    ///
    /// [`split_at_mut`]: #method.split_at_mut
    /// [undefined behavior]: https://doc.rust-lang.org/reference/behavior-considered-undefined.html
    ///
    /// # Examples
    ///
    /// ```compile_fail
    /// #![feature(slice_split_at_unchecked)]
    ///
    /// let mut v = [1, 0, 3, 0, 5, 6];
    /// // scoped to restrict the lifetime of the borrows
    /// unsafe {
    ///     let (left, right) = v.split_at_mut_unchecked(2);
    ///     assert_eq!(left, [1, 0]);
    ///     assert_eq!(right, [3, 0, 5, 6]);
    ///     left[1] = 2;
    ///     right[1] = 4;
    /// }
    /// assert_eq!(v, [1, 2, 3, 4, 5, 6]);
    /// ```
    #[unstable(feature = "slice_split_at_unchecked", reason = "new API", issue = "76014")]
    #[inline]
    unsafe fn split_at_mut_unchecked(&mut self, mid: usize) -> (&mut [T], &mut [T]) {
        let len = self.len();
        let ptr = self.as_mut_ptr();

        // SAFETY: Caller has to check that `0 <= mid <= self.len()`.
        //
        // `[ptr; mid]` and `[mid; len]` are not overlapping, so returning a mutable reference
        // is fine.
        unsafe { (from_raw_parts_mut(ptr, mid), from_raw_parts_mut(ptr.add(mid), len - mid)) }
    }

    /// Returns an iterator over subslices separated by elements that match
    /// `pred`. The matched element is not contained in the subslices.
    ///
    /// # Examples
    ///
    /// ```
    /// let slice = [10, 40, 33, 20];
    /// let mut iter = slice.split(|num| num % 3 == 0);
    ///
    /// assert_eq!(iter.next().unwrap(), &[10, 40]);
    /// assert_eq!(iter.next().unwrap(), &[20]);
    /// assert!(iter.next().is_none());
    /// ```
    ///
    /// If the first element is matched, an empty slice will be the first item
    /// returned by the iterator. Similarly, if the last element in the slice
    /// is matched, an empty slice will be the last item returned by the
    /// iterator:
    ///
    /// ```
    /// let slice = [10, 40, 33];
    /// let mut iter = slice.split(|num| num % 3 == 0);
    ///
    /// assert_eq!(iter.next().unwrap(), &[10, 40]);
    /// assert_eq!(iter.next().unwrap(), &[]);
    /// assert!(iter.next().is_none());
    /// ```
    ///
    /// If two matched elements are directly adjacent, an empty slice will be
    /// present between them:
    ///
    /// ```
    /// let slice = [10, 6, 33, 20];
    /// let mut iter = slice.split(|num| num % 3 == 0);
    ///
    /// assert_eq!(iter.next().unwrap(), &[10]);
    /// assert_eq!(iter.next().unwrap(), &[]);
    /// assert_eq!(iter.next().unwrap(), &[20]);
    /// assert!(iter.next().is_none());
    /// ```
    #[stable(feature = "rust1", since = "1.0.0")]
    #[inline]
    pub fn split<F>(&self, pred: F) -> Split<'_, T, F>
    where
        F: FnMut(&T) -> bool,
    {
        Split::new(self, pred)
    }

    /// Returns an iterator over mutable subslices separated by elements that
    /// match `pred`. The matched element is not contained in the subslices.
    ///
    /// # Examples
    ///
    /// ```
    /// let mut v = [10, 40, 30, 20, 60, 50];
    ///
    /// for group in v.split_mut(|num| *num % 3 == 0) {
    ///     group[0] = 1;
    /// }
    /// assert_eq!(v, [1, 40, 30, 1, 60, 1]);
    /// ```
    #[stable(feature = "rust1", since = "1.0.0")]
    #[inline]
    pub fn split_mut<F>(&mut self, pred: F) -> SplitMut<'_, T, F>
    where
        F: FnMut(&T) -> bool,
    {
        SplitMut::new(self, pred)
    }

    /// Returns an iterator over subslices separated by elements that match
    /// `pred`. The matched element is contained in the end of the previous
    /// subslice as a terminator.
    ///
    /// # Examples
    ///
    /// ```
    /// #![feature(split_inclusive)]
    /// let slice = [10, 40, 33, 20];
    /// let mut iter = slice.split_inclusive(|num| num % 3 == 0);
    ///
    /// assert_eq!(iter.next().unwrap(), &[10, 40, 33]);
    /// assert_eq!(iter.next().unwrap(), &[20]);
    /// assert!(iter.next().is_none());
    /// ```
    ///
    /// If the last element of the slice is matched,
    /// that element will be considered the terminator of the preceding slice.
    /// That slice will be the last item returned by the iterator.
    ///
    /// ```
    /// #![feature(split_inclusive)]
    /// let slice = [3, 10, 40, 33];
    /// let mut iter = slice.split_inclusive(|num| num % 3 == 0);
    ///
    /// assert_eq!(iter.next().unwrap(), &[3]);
    /// assert_eq!(iter.next().unwrap(), &[10, 40, 33]);
    /// assert!(iter.next().is_none());
    /// ```
    #[unstable(feature = "split_inclusive", issue = "72360")]
    #[inline]
    pub fn split_inclusive<F>(&self, pred: F) -> SplitInclusive<'_, T, F>
    where
        F: FnMut(&T) -> bool,
    {
        SplitInclusive::new(self, pred)
    }

    /// Returns an iterator over mutable subslices separated by elements that
    /// match `pred`. The matched element is contained in the previous
    /// subslice as a terminator.
    ///
    /// # Examples
    ///
    /// ```
    /// #![feature(split_inclusive)]
    /// let mut v = [10, 40, 30, 20, 60, 50];
    ///
    /// for group in v.split_inclusive_mut(|num| *num % 3 == 0) {
    ///     let terminator_idx = group.len()-1;
    ///     group[terminator_idx] = 1;
    /// }
    /// assert_eq!(v, [10, 40, 1, 20, 1, 1]);
    /// ```
    #[unstable(feature = "split_inclusive", issue = "72360")]
    #[inline]
    pub fn split_inclusive_mut<F>(&mut self, pred: F) -> SplitInclusiveMut<'_, T, F>
    where
        F: FnMut(&T) -> bool,
    {
        SplitInclusiveMut::new(self, pred)
    }

    /// Returns an iterator over subslices separated by elements that match
    /// `pred`, starting at the end of the slice and working backwards.
    /// The matched element is not contained in the subslices.
    ///
    /// # Examples
    ///
    /// ```
    /// let slice = [11, 22, 33, 0, 44, 55];
    /// let mut iter = slice.rsplit(|num| *num == 0);
    ///
    /// assert_eq!(iter.next().unwrap(), &[44, 55]);
    /// assert_eq!(iter.next().unwrap(), &[11, 22, 33]);
    /// assert_eq!(iter.next(), None);
    /// ```
    ///
    /// As with `split()`, if the first or last element is matched, an empty
    /// slice will be the first (or last) item returned by the iterator.
    ///
    /// ```
    /// let v = &[0, 1, 1, 2, 3, 5, 8];
    /// let mut it = v.rsplit(|n| *n % 2 == 0);
    /// assert_eq!(it.next().unwrap(), &[]);
    /// assert_eq!(it.next().unwrap(), &[3, 5]);
    /// assert_eq!(it.next().unwrap(), &[1, 1]);
    /// assert_eq!(it.next().unwrap(), &[]);
    /// assert_eq!(it.next(), None);
    /// ```
    #[stable(feature = "slice_rsplit", since = "1.27.0")]
    #[inline]
    pub fn rsplit<F>(&self, pred: F) -> RSplit<'_, T, F>
    where
        F: FnMut(&T) -> bool,
    {
        RSplit::new(self, pred)
    }

    /// Returns an iterator over mutable subslices separated by elements that
    /// match `pred`, starting at the end of the slice and working
    /// backwards. The matched element is not contained in the subslices.
    ///
    /// # Examples
    ///
    /// ```
    /// let mut v = [100, 400, 300, 200, 600, 500];
    ///
    /// let mut count = 0;
    /// for group in v.rsplit_mut(|num| *num % 3 == 0) {
    ///     count += 1;
    ///     group[0] = count;
    /// }
    /// assert_eq!(v, [3, 400, 300, 2, 600, 1]);
    /// ```
    ///
    #[stable(feature = "slice_rsplit", since = "1.27.0")]
    #[inline]
    pub fn rsplit_mut<F>(&mut self, pred: F) -> RSplitMut<'_, T, F>
    where
        F: FnMut(&T) -> bool,
    {
        RSplitMut::new(self, pred)
    }

    /// Returns an iterator over subslices separated by elements that match
    /// `pred`, limited to returning at most `n` items. The matched element is
    /// not contained in the subslices.
    ///
    /// The last element returned, if any, will contain the remainder of the
    /// slice.
    ///
    /// # Examples
    ///
    /// Print the slice split once by numbers divisible by 3 (i.e., `[10, 40]`,
    /// `[20, 60, 50]`):
    ///
    /// ```
    /// let v = [10, 40, 30, 20, 60, 50];
    ///
    /// for group in v.splitn(2, |num| *num % 3 == 0) {
    ///     println!("{:?}", group);
    /// }
    /// ```
    #[stable(feature = "rust1", since = "1.0.0")]
    #[inline]
    pub fn splitn<F>(&self, n: usize, pred: F) -> SplitN<'_, T, F>
    where
        F: FnMut(&T) -> bool,
    {
        SplitN::new(self.split(pred), n)
    }

    /// Returns an iterator over subslices separated by elements that match
    /// `pred`, limited to returning at most `n` items. The matched element is
    /// not contained in the subslices.
    ///
    /// The last element returned, if any, will contain the remainder of the
    /// slice.
    ///
    /// # Examples
    ///
    /// ```
    /// let mut v = [10, 40, 30, 20, 60, 50];
    ///
    /// for group in v.splitn_mut(2, |num| *num % 3 == 0) {
    ///     group[0] = 1;
    /// }
    /// assert_eq!(v, [1, 40, 30, 1, 60, 50]);
    /// ```
    #[stable(feature = "rust1", since = "1.0.0")]
    #[inline]
    pub fn splitn_mut<F>(&mut self, n: usize, pred: F) -> SplitNMut<'_, T, F>
    where
        F: FnMut(&T) -> bool,
    {
        SplitNMut::new(self.split_mut(pred), n)
    }

    /// Returns an iterator over subslices separated by elements that match
    /// `pred` limited to returning at most `n` items. This starts at the end of
    /// the slice and works backwards. The matched element is not contained in
    /// the subslices.
    ///
    /// The last element returned, if any, will contain the remainder of the
    /// slice.
    ///
    /// # Examples
    ///
    /// Print the slice split once, starting from the end, by numbers divisible
    /// by 3 (i.e., `[50]`, `[10, 40, 30, 20]`):
    ///
    /// ```
    /// let v = [10, 40, 30, 20, 60, 50];
    ///
    /// for group in v.rsplitn(2, |num| *num % 3 == 0) {
    ///     println!("{:?}", group);
    /// }
    /// ```
    #[stable(feature = "rust1", since = "1.0.0")]
    #[inline]
    pub fn rsplitn<F>(&self, n: usize, pred: F) -> RSplitN<'_, T, F>
    where
        F: FnMut(&T) -> bool,
    {
        RSplitN::new(self.rsplit(pred), n)
    }

    /// Returns an iterator over subslices separated by elements that match
    /// `pred` limited to returning at most `n` items. This starts at the end of
    /// the slice and works backwards. The matched element is not contained in
    /// the subslices.
    ///
    /// The last element returned, if any, will contain the remainder of the
    /// slice.
    ///
    /// # Examples
    ///
    /// ```
    /// let mut s = [10, 40, 30, 20, 60, 50];
    ///
    /// for group in s.rsplitn_mut(2, |num| *num % 3 == 0) {
    ///     group[0] = 1;
    /// }
    /// assert_eq!(s, [1, 40, 30, 20, 60, 1]);
    /// ```
    #[stable(feature = "rust1", since = "1.0.0")]
    #[inline]
    pub fn rsplitn_mut<F>(&mut self, n: usize, pred: F) -> RSplitNMut<'_, T, F>
    where
        F: FnMut(&T) -> bool,
    {
        RSplitNMut::new(self.rsplit_mut(pred), n)
    }

    /// Returns `true` if the slice contains an element with the given value.
    ///
    /// # Examples
    ///
    /// ```
    /// let v = [10, 40, 30];
    /// assert!(v.contains(&30));
    /// assert!(!v.contains(&50));
    /// ```
    ///
    /// If you do not have an `&T`, but just an `&U` such that `T: Borrow<U>`
    /// (e.g. `String: Borrow<str>`), you can use `iter().any`:
    ///
    /// ```
    /// let v = [String::from("hello"), String::from("world")]; // slice of `String`
    /// assert!(v.iter().any(|e| e == "hello")); // search with `&str`
    /// assert!(!v.iter().any(|e| e == "hi"));
    /// ```
    #[stable(feature = "rust1", since = "1.0.0")]
    #[inline]
    pub fn contains(&self, x: &T) -> bool
    where
        T: PartialEq,
    {
        cmp::SliceContains::slice_contains(x, self)
    }

    /// Returns `true` if `needle` is a prefix of the slice.
    ///
    /// # Examples
    ///
    /// ```
    /// let v = [10, 40, 30];
    /// assert!(v.starts_with(&[10]));
    /// assert!(v.starts_with(&[10, 40]));
    /// assert!(!v.starts_with(&[50]));
    /// assert!(!v.starts_with(&[10, 50]));
    /// ```
    ///
    /// Always returns `true` if `needle` is an empty slice:
    ///
    /// ```
    /// let v = &[10, 40, 30];
    /// assert!(v.starts_with(&[]));
    /// let v: &[u8] = &[];
    /// assert!(v.starts_with(&[]));
    /// ```
    #[stable(feature = "rust1", since = "1.0.0")]
    pub fn starts_with(&self, needle: &[T]) -> bool
    where
        T: PartialEq,
    {
        let n = needle.len();
        self.len() >= n && needle == &self[..n]
    }

    /// Returns `true` if `needle` is a suffix of the slice.
    ///
    /// # Examples
    ///
    /// ```
    /// let v = [10, 40, 30];
    /// assert!(v.ends_with(&[30]));
    /// assert!(v.ends_with(&[40, 30]));
    /// assert!(!v.ends_with(&[50]));
    /// assert!(!v.ends_with(&[50, 30]));
    /// ```
    ///
    /// Always returns `true` if `needle` is an empty slice:
    ///
    /// ```
    /// let v = &[10, 40, 30];
    /// assert!(v.ends_with(&[]));
    /// let v: &[u8] = &[];
    /// assert!(v.ends_with(&[]));
    /// ```
    #[stable(feature = "rust1", since = "1.0.0")]
    pub fn ends_with(&self, needle: &[T]) -> bool
    where
        T: PartialEq,
    {
        let (m, n) = (self.len(), needle.len());
        m >= n && needle == &self[m - n..]
    }

    /// Returns a subslice with the prefix removed.
    ///
    /// If the slice starts with `prefix`, returns the subslice after the prefix, wrapped in `Some`.
    /// If `prefix` is empty, simply returns the original slice.
    ///
    /// If the slice does not start with `prefix`, returns `None`.
    ///
    /// # Examples
    ///
    /// ```
    /// #![feature(slice_strip)]
    /// let v = &[10, 40, 30];
    /// assert_eq!(v.strip_prefix(&[10]), Some(&[40, 30][..]));
    /// assert_eq!(v.strip_prefix(&[10, 40]), Some(&[30][..]));
    /// assert_eq!(v.strip_prefix(&[50]), None);
    /// assert_eq!(v.strip_prefix(&[10, 50]), None);
    /// ```
    #[must_use = "returns the subslice without modifying the original"]
    #[unstable(feature = "slice_strip", issue = "73413")]
    pub fn strip_prefix(&self, prefix: &[T]) -> Option<&[T]>
    where
        T: PartialEq,
    {
        let n = prefix.len();
        if n <= self.len() {
            let (head, tail) = self.split_at(n);
            if head == prefix {
                return Some(tail);
            }
        }
        None
    }

    /// Returns a subslice with the suffix removed.
    ///
    /// If the slice ends with `suffix`, returns the subslice before the suffix, wrapped in `Some`.
    /// If `suffix` is empty, simply returns the original slice.
    ///
    /// If the slice does not end with `suffix`, returns `None`.
    ///
    /// # Examples
    ///
    /// ```
    /// #![feature(slice_strip)]
    /// let v = &[10, 40, 30];
    /// assert_eq!(v.strip_suffix(&[30]), Some(&[10, 40][..]));
    /// assert_eq!(v.strip_suffix(&[40, 30]), Some(&[10][..]));
    /// assert_eq!(v.strip_suffix(&[50]), None);
    /// assert_eq!(v.strip_suffix(&[50, 30]), None);
    /// ```
    #[must_use = "returns the subslice without modifying the original"]
    #[unstable(feature = "slice_strip", issue = "73413")]
    pub fn strip_suffix(&self, suffix: &[T]) -> Option<&[T]>
    where
        T: PartialEq,
    {
        let (len, n) = (self.len(), suffix.len());
        if n <= len {
            let (head, tail) = self.split_at(len - n);
            if tail == suffix {
                return Some(head);
            }
        }
        None
    }

    /// Binary searches this sorted slice for a given element.
    ///
    /// If the value is found then [`Result::Ok`] is returned, containing the
    /// index of the matching element. If there are multiple matches, then any
    /// one of the matches could be returned. If the value is not found then
    /// [`Result::Err`] is returned, containing the index where a matching
    /// element could be inserted while maintaining sorted order.
    ///
    /// # Examples
    ///
    /// Looks up a series of four elements. The first is found, with a
    /// uniquely determined position; the second and third are not
    /// found; the fourth could match any position in `[1, 4]`.
    ///
    /// ```
    /// let s = [0, 1, 1, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55];
    ///
    /// assert_eq!(s.binary_search(&13),  Ok(9));
    /// assert_eq!(s.binary_search(&4),   Err(7));
    /// assert_eq!(s.binary_search(&100), Err(13));
    /// let r = s.binary_search(&1);
    /// assert!(match r { Ok(1..=4) => true, _ => false, });
    /// ```
    ///
    /// If you want to insert an item to a sorted vector, while maintaining
    /// sort order:
    ///
    /// ```
    /// let mut s = vec![0, 1, 1, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55];
    /// let num = 42;
    /// let idx = s.binary_search(&num).unwrap_or_else(|x| x);
    /// s.insert(idx, num);
    /// assert_eq!(s, [0, 1, 1, 1, 1, 2, 3, 5, 8, 13, 21, 34, 42, 55]);
    /// ```
    #[stable(feature = "rust1", since = "1.0.0")]
    pub fn binary_search(&self, x: &T) -> Result<usize, usize>
    where
        T: Ord,
    {
        self.binary_search_by(|p| p.cmp(x))
    }

    /// Binary searches this sorted slice with a comparator function.
    ///
    /// The comparator function should implement an order consistent
    /// with the sort order of the underlying slice, returning an
    /// order code that indicates whether its argument is `Less`,
    /// `Equal` or `Greater` the desired target.
    ///
    /// If the value is found then [`Result::Ok`] is returned, containing the
    /// index of the matching element. If there are multiple matches, then any
    /// one of the matches could be returned. If the value is not found then
    /// [`Result::Err`] is returned, containing the index where a matching
    /// element could be inserted while maintaining sorted order.
    ///
    /// # Examples
    ///
    /// Looks up a series of four elements. The first is found, with a
    /// uniquely determined position; the second and third are not
    /// found; the fourth could match any position in `[1, 4]`.
    ///
    /// ```
    /// let s = [0, 1, 1, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55];
    ///
    /// let seek = 13;
    /// assert_eq!(s.binary_search_by(|probe| probe.cmp(&seek)), Ok(9));
    /// let seek = 4;
    /// assert_eq!(s.binary_search_by(|probe| probe.cmp(&seek)), Err(7));
    /// let seek = 100;
    /// assert_eq!(s.binary_search_by(|probe| probe.cmp(&seek)), Err(13));
    /// let seek = 1;
    /// let r = s.binary_search_by(|probe| probe.cmp(&seek));
    /// assert!(match r { Ok(1..=4) => true, _ => false, });
    /// ```
    #[stable(feature = "rust1", since = "1.0.0")]
    #[inline]
    pub fn binary_search_by<'a, F>(&'a self, mut f: F) -> Result<usize, usize>
    where
        F: FnMut(&'a T) -> Ordering,
    {
        let s = self;
        let mut size = s.len();
        if size == 0 {
            return Err(0);
        }
        let mut base = 0usize;
        while size > 1 {
            let half = size / 2;
            let mid = base + half;
            // SAFETY: the call is made safe by the following inconstants:
            // - `mid >= 0`: by definition
            // - `mid < size`: `mid = size / 2 + size / 4 + size / 8 ...`
            let cmp = f(unsafe { s.get_unchecked(mid) });
            base = if cmp == Greater { base } else { mid };
            size -= half;
        }
        // SAFETY: base is always in [0, size) because base <= mid.
        let cmp = f(unsafe { s.get_unchecked(base) });
        if cmp == Equal { Ok(base) } else { Err(base + (cmp == Less) as usize) }
    }

    /// Binary searches this sorted slice with a key extraction function.
    ///
    /// Assumes that the slice is sorted by the key, for instance with
    /// [`sort_by_key`] using the same key extraction function.
    ///
    /// If the value is found then [`Result::Ok`] is returned, containing the
    /// index of the matching element. If there are multiple matches, then any
    /// one of the matches could be returned. If the value is not found then
    /// [`Result::Err`] is returned, containing the index where a matching
    /// element could be inserted while maintaining sorted order.
    ///
    /// [`sort_by_key`]: #method.sort_by_key
    ///
    /// # Examples
    ///
    /// Looks up a series of four elements in a slice of pairs sorted by
    /// their second elements. The first is found, with a uniquely
    /// determined position; the second and third are not found; the
    /// fourth could match any position in `[1, 4]`.
    ///
    /// ```
    /// let s = [(0, 0), (2, 1), (4, 1), (5, 1), (3, 1),
    ///          (1, 2), (2, 3), (4, 5), (5, 8), (3, 13),
    ///          (1, 21), (2, 34), (4, 55)];
    ///
    /// assert_eq!(s.binary_search_by_key(&13, |&(a,b)| b),  Ok(9));
    /// assert_eq!(s.binary_search_by_key(&4, |&(a,b)| b),   Err(7));
    /// assert_eq!(s.binary_search_by_key(&100, |&(a,b)| b), Err(13));
    /// let r = s.binary_search_by_key(&1, |&(a,b)| b);
    /// assert!(match r { Ok(1..=4) => true, _ => false, });
    /// ```
    #[stable(feature = "slice_binary_search_by_key", since = "1.10.0")]
    #[inline]
    pub fn binary_search_by_key<'a, B, F>(&'a self, b: &B, mut f: F) -> Result<usize, usize>
    where
        F: FnMut(&'a T) -> B,
        B: Ord,
    {
        self.binary_search_by(|k| f(k).cmp(b))
    }

    /// Sorts the slice, but may not preserve the order of equal elements.
    ///
    /// This sort is unstable (i.e., may reorder equal elements), in-place
    /// (i.e., does not allocate), and *O*(*n* \* log(*n*)) worst-case.
    ///
    /// # Current implementation
    ///
    /// The current algorithm is based on [pattern-defeating quicksort][pdqsort] by Orson Peters,
    /// which combines the fast average case of randomized quicksort with the fast worst case of
    /// heapsort, while achieving linear time on slices with certain patterns. It uses some
    /// randomization to avoid degenerate cases, but with a fixed seed to always provide
    /// deterministic behavior.
    ///
    /// It is typically faster than stable sorting, except in a few special cases, e.g., when the
    /// slice consists of several concatenated sorted sequences.
    ///
    /// # Examples
    ///
    /// ```
    /// let mut v = [-5, 4, 1, -3, 2];
    ///
    /// v.sort_unstable();
    /// assert!(v == [-5, -3, 1, 2, 4]);
    /// ```
    ///
    /// [pdqsort]: https://github.com/orlp/pdqsort
    #[stable(feature = "sort_unstable", since = "1.20.0")]
    #[inline]
    pub fn sort_unstable(&mut self)
    where
        T: Ord,
    {
        sort::quicksort(self, |a, b| a.lt(b));
    }

    /// Sorts the slice with a comparator function, but may not preserve the order of equal
    /// elements.
    ///
    /// This sort is unstable (i.e., may reorder equal elements), in-place
    /// (i.e., does not allocate), and *O*(*n* \* log(*n*)) worst-case.
    ///
    /// The comparator function must define a total ordering for the elements in the slice. If
    /// the ordering is not total, the order of the elements is unspecified. An order is a
    /// total order if it is (for all `a`, `b` and `c`):
    ///
    /// * total and antisymmetric: exactly one of `a < b`, `a == b` or `a > b` is true, and
    /// * transitive, `a < b` and `b < c` implies `a < c`. The same must hold for both `==` and `>`.
    ///
    /// For example, while [`f64`] doesn't implement [`Ord`] because `NaN != NaN`, we can use
    /// `partial_cmp` as our sort function when we know the slice doesn't contain a `NaN`.
    ///
    /// ```
    /// let mut floats = [5f64, 4.0, 1.0, 3.0, 2.0];
    /// floats.sort_unstable_by(|a, b| a.partial_cmp(b).unwrap());
    /// assert_eq!(floats, [1.0, 2.0, 3.0, 4.0, 5.0]);
    /// ```
    ///
    /// # Current implementation
    ///
    /// The current algorithm is based on [pattern-defeating quicksort][pdqsort] by Orson Peters,
    /// which combines the fast average case of randomized quicksort with the fast worst case of
    /// heapsort, while achieving linear time on slices with certain patterns. It uses some
    /// randomization to avoid degenerate cases, but with a fixed seed to always provide
    /// deterministic behavior.
    ///
    /// It is typically faster than stable sorting, except in a few special cases, e.g., when the
    /// slice consists of several concatenated sorted sequences.
    ///
    /// # Examples
    ///
    /// ```
    /// let mut v = [5, 4, 1, 3, 2];
    /// v.sort_unstable_by(|a, b| a.cmp(b));
    /// assert!(v == [1, 2, 3, 4, 5]);
    ///
    /// // reverse sorting
    /// v.sort_unstable_by(|a, b| b.cmp(a));
    /// assert!(v == [5, 4, 3, 2, 1]);
    /// ```
    ///
    /// [pdqsort]: https://github.com/orlp/pdqsort
    #[stable(feature = "sort_unstable", since = "1.20.0")]
    #[inline]
    pub fn sort_unstable_by<F>(&mut self, mut compare: F)
    where
        F: FnMut(&T, &T) -> Ordering,
    {
        sort::quicksort(self, |a, b| compare(a, b) == Ordering::Less);
    }

    /// Sorts the slice with a key extraction function, but may not preserve the order of equal
    /// elements.
    ///
    /// This sort is unstable (i.e., may reorder equal elements), in-place
    /// (i.e., does not allocate), and *O*(m \* *n* \* log(*n*)) worst-case, where the key function is
    /// *O*(*m*).
    ///
    /// # Current implementation
    ///
    /// The current algorithm is based on [pattern-defeating quicksort][pdqsort] by Orson Peters,
    /// which combines the fast average case of randomized quicksort with the fast worst case of
    /// heapsort, while achieving linear time on slices with certain patterns. It uses some
    /// randomization to avoid degenerate cases, but with a fixed seed to always provide
    /// deterministic behavior.
    ///
    /// Due to its key calling strategy, [`sort_unstable_by_key`](#method.sort_unstable_by_key)
    /// is likely to be slower than [`sort_by_cached_key`](#method.sort_by_cached_key) in
    /// cases where the key function is expensive.
    ///
    /// # Examples
    ///
    /// ```
    /// let mut v = [-5i32, 4, 1, -3, 2];
    ///
    /// v.sort_unstable_by_key(|k| k.abs());
    /// assert!(v == [1, 2, -3, 4, -5]);
    /// ```
    ///
    /// [pdqsort]: https://github.com/orlp/pdqsort
    #[stable(feature = "sort_unstable", since = "1.20.0")]
    #[inline]
    pub fn sort_unstable_by_key<K, F>(&mut self, mut f: F)
    where
        F: FnMut(&T) -> K,
        K: Ord,
    {
        sort::quicksort(self, |a, b| f(a).lt(&f(b)));
    }

    /// Reorder the slice such that the element at `index` is at its final sorted position.
    #[unstable(feature = "slice_partition_at_index", issue = "55300")]
    #[rustc_deprecated(since = "1.49.0", reason = "use the select_nth_unstable() instead")]
    #[inline]
    pub fn partition_at_index(&mut self, index: usize) -> (&mut [T], &mut T, &mut [T])
    where
        T: Ord,
    {
        self.select_nth_unstable(index)
    }

    /// Reorder the slice with a comparator function such that the element at `index` is at its
    /// final sorted position.
    #[unstable(feature = "slice_partition_at_index", issue = "55300")]
    #[rustc_deprecated(since = "1.49.0", reason = "use select_nth_unstable_by() instead")]
    #[inline]
    pub fn partition_at_index_by<F>(
        &mut self,
        index: usize,
        compare: F,
    ) -> (&mut [T], &mut T, &mut [T])
    where
        F: FnMut(&T, &T) -> Ordering,
    {
        self.select_nth_unstable_by(index, compare)
    }

    /// Reorder the slice with a key extraction function such that the element at `index` is at its
    /// final sorted position.
    #[unstable(feature = "slice_partition_at_index", issue = "55300")]
    #[rustc_deprecated(since = "1.49.0", reason = "use the select_nth_unstable_by_key() instead")]
    #[inline]
    pub fn partition_at_index_by_key<K, F>(
        &mut self,
        index: usize,
        f: F,
    ) -> (&mut [T], &mut T, &mut [T])
    where
        F: FnMut(&T) -> K,
        K: Ord,
    {
        self.select_nth_unstable_by_key(index, f)
    }

    /// Reorder the slice such that the element at `index` is at its final sorted position.
    ///
    /// This reordering has the additional property that any value at position `i < index` will be
    /// less than or equal to any value at a position `j > index`. Additionally, this reordering is
    /// unstable (i.e. any number of equal elements may end up at position `index`), in-place
    /// (i.e. does not allocate), and *O*(*n*) worst-case. This function is also/ known as "kth
    /// element" in other libraries. It returns a triplet of the following values: all elements less
    /// than the one at the given index, the value at the given index, and all elements greater than
    /// the one at the given index.
    ///
    /// # Current implementation
    ///
    /// The current algorithm is based on the quickselect portion of the same quicksort algorithm
    /// used for [`sort_unstable`].
    ///
    /// [`sort_unstable`]: #method.sort_unstable
    ///
    /// # Panics
    ///
    /// Panics when `index >= len()`, meaning it always panics on empty slices.
    ///
    /// # Examples
    ///
    /// ```
    /// let mut v = [-5i32, 4, 1, -3, 2];
    ///
    /// // Find the median
    /// v.select_nth_unstable(2);
    ///
    /// // We are only guaranteed the slice will be one of the following, based on the way we sort
    /// // about the specified index.
    /// assert!(v == [-3, -5, 1, 2, 4] ||
    ///         v == [-5, -3, 1, 2, 4] ||
    ///         v == [-3, -5, 1, 4, 2] ||
    ///         v == [-5, -3, 1, 4, 2]);
    /// ```
    #[stable(feature = "slice_select_nth_unstable", since = "1.49.0")]
    #[inline]
    pub fn select_nth_unstable(&mut self, index: usize) -> (&mut [T], &mut T, &mut [T])
    where
        T: Ord,
    {
        let mut f = |a: &T, b: &T| a.lt(b);
        sort::partition_at_index(self, index, &mut f)
    }

    /// Reorder the slice with a comparator function such that the element at `index` is at its
    /// final sorted position.
    ///
    /// This reordering has the additional property that any value at position `i < index` will be
    /// less than or equal to any value at a position `j > index` using the comparator function.
    /// Additionally, this reordering is unstable (i.e. any number of equal elements may end up at
    /// position `index`), in-place (i.e. does not allocate), and *O*(*n*) worst-case. This function
    /// is also known as "kth element" in other libraries. It returns a triplet of the following
    /// values: all elements less than the one at the given index, the value at the given index,
    /// and all elements greater than the one at the given index, using the provided comparator
    /// function.
    ///
    /// # Current implementation
    ///
    /// The current algorithm is based on the quickselect portion of the same quicksort algorithm
    /// used for [`sort_unstable`].
    ///
    /// [`sort_unstable`]: #method.sort_unstable
    ///
    /// # Panics
    ///
    /// Panics when `index >= len()`, meaning it always panics on empty slices.
    ///
    /// # Examples
    ///
    /// ```
    /// let mut v = [-5i32, 4, 1, -3, 2];
    ///
    /// // Find the median as if the slice were sorted in descending order.
    /// v.select_nth_unstable_by(2, |a, b| b.cmp(a));
    ///
    /// // We are only guaranteed the slice will be one of the following, based on the way we sort
    /// // about the specified index.
    /// assert!(v == [2, 4, 1, -5, -3] ||
    ///         v == [2, 4, 1, -3, -5] ||
    ///         v == [4, 2, 1, -5, -3] ||
    ///         v == [4, 2, 1, -3, -5]);
    /// ```
    #[stable(feature = "slice_select_nth_unstable", since = "1.49.0")]
    #[inline]
    pub fn select_nth_unstable_by<F>(
        &mut self,
        index: usize,
        mut compare: F,
    ) -> (&mut [T], &mut T, &mut [T])
    where
        F: FnMut(&T, &T) -> Ordering,
    {
        let mut f = |a: &T, b: &T| compare(a, b) == Less;
        sort::partition_at_index(self, index, &mut f)
    }

    /// Reorder the slice with a key extraction function such that the element at `index` is at its
    /// final sorted position.
    ///
    /// This reordering has the additional property that any value at position `i < index` will be
    /// less than or equal to any value at a position `j > index` using the key extraction function.
    /// Additionally, this reordering is unstable (i.e. any number of equal elements may end up at
    /// position `index`), in-place (i.e. does not allocate), and *O*(*n*) worst-case. This function
    /// is also known as "kth element" in other libraries. It returns a triplet of the following
    /// values: all elements less than the one at the given index, the value at the given index, and
    /// all elements greater than the one at the given index, using the provided key extraction
    /// function.
    ///
    /// # Current implementation
    ///
    /// The current algorithm is based on the quickselect portion of the same quicksort algorithm
    /// used for [`sort_unstable`].
    ///
    /// [`sort_unstable`]: #method.sort_unstable
    ///
    /// # Panics
    ///
    /// Panics when `index >= len()`, meaning it always panics on empty slices.
    ///
    /// # Examples
    ///
    /// ```
    /// let mut v = [-5i32, 4, 1, -3, 2];
    ///
    /// // Return the median as if the array were sorted according to absolute value.
    /// v.select_nth_unstable_by_key(2, |a| a.abs());
    ///
    /// // We are only guaranteed the slice will be one of the following, based on the way we sort
    /// // about the specified index.
    /// assert!(v == [1, 2, -3, 4, -5] ||
    ///         v == [1, 2, -3, -5, 4] ||
    ///         v == [2, 1, -3, 4, -5] ||
    ///         v == [2, 1, -3, -5, 4]);
    /// ```
    #[stable(feature = "slice_select_nth_unstable", since = "1.49.0")]
    #[inline]
    pub fn select_nth_unstable_by_key<K, F>(
        &mut self,
        index: usize,
        mut f: F,
    ) -> (&mut [T], &mut T, &mut [T])
    where
        F: FnMut(&T) -> K,
        K: Ord,
    {
        let mut g = |a: &T, b: &T| f(a).lt(&f(b));
        sort::partition_at_index(self, index, &mut g)
    }

    /// Moves all consecutive repeated elements to the end of the slice according to the
    /// [`PartialEq`] trait implementation.
    ///
    /// Returns two slices. The first contains no consecutive repeated elements.
    /// The second contains all the duplicates in no specified order.
    ///
    /// If the slice is sorted, the first returned slice contains no duplicates.
    ///
    /// # Examples
    ///
    /// ```
    /// #![feature(slice_partition_dedup)]
    ///
    /// let mut slice = [1, 2, 2, 3, 3, 2, 1, 1];
    ///
    /// let (dedup, duplicates) = slice.partition_dedup();
    ///
    /// assert_eq!(dedup, [1, 2, 3, 2, 1]);
    /// assert_eq!(duplicates, [2, 3, 1]);
    /// ```
    #[unstable(feature = "slice_partition_dedup", issue = "54279")]
    #[inline]
    pub fn partition_dedup(&mut self) -> (&mut [T], &mut [T])
    where
        T: PartialEq,
    {
        self.partition_dedup_by(|a, b| a == b)
    }

    /// Moves all but the first of consecutive elements to the end of the slice satisfying
    /// a given equality relation.
    ///
    /// Returns two slices. The first contains no consecutive repeated elements.
    /// The second contains all the duplicates in no specified order.
    ///
    /// The `same_bucket` function is passed references to two elements from the slice and
    /// must determine if the elements compare equal. The elements are passed in opposite order
    /// from their order in the slice, so if `same_bucket(a, b)` returns `true`, `a` is moved
    /// at the end of the slice.
    ///
    /// If the slice is sorted, the first returned slice contains no duplicates.
    ///
    /// # Examples
    ///
    /// ```
    /// #![feature(slice_partition_dedup)]
    ///
    /// let mut slice = ["foo", "Foo", "BAZ", "Bar", "bar", "baz", "BAZ"];
    ///
    /// let (dedup, duplicates) = slice.partition_dedup_by(|a, b| a.eq_ignore_ascii_case(b));
    ///
    /// assert_eq!(dedup, ["foo", "BAZ", "Bar", "baz"]);
    /// assert_eq!(duplicates, ["bar", "Foo", "BAZ"]);
    /// ```
    #[unstable(feature = "slice_partition_dedup", issue = "54279")]
    #[inline]
    pub fn partition_dedup_by<F>(&mut self, mut same_bucket: F) -> (&mut [T], &mut [T])
    where
        F: FnMut(&mut T, &mut T) -> bool,
    {
        // Although we have a mutable reference to `self`, we cannot make
        // *arbitrary* changes. The `same_bucket` calls could panic, so we
        // must ensure that the slice is in a valid state at all times.
        //
        // The way that we handle this is by using swaps; we iterate
        // over all the elements, swapping as we go so that at the end
        // the elements we wish to keep are in the front, and those we
        // wish to reject are at the back. We can then split the slice.
        // This operation is still `O(n)`.
        //
        // Example: We start in this state, where `r` represents "next
        // read" and `w` represents "next_write`.
        //
        //           r
        //     +---+---+---+---+---+---+
        //     | 0 | 1 | 1 | 2 | 3 | 3 |
        //     +---+---+---+---+---+---+
        //           w
        //
        // Comparing self[r] against self[w-1], this is not a duplicate, so
        // we swap self[r] and self[w] (no effect as r==w) and then increment both
        // r and w, leaving us with:
        //
        //               r
        //     +---+---+---+---+---+---+
        //     | 0 | 1 | 1 | 2 | 3 | 3 |
        //     +---+---+---+---+---+---+
        //               w
        //
        // Comparing self[r] against self[w-1], this value is a duplicate,
        // so we increment `r` but leave everything else unchanged:
        //
        //                   r
        //     +---+---+---+---+---+---+
        //     | 0 | 1 | 1 | 2 | 3 | 3 |
        //     +---+---+---+---+---+---+
        //               w
        //
        // Comparing self[r] against self[w-1], this is not a duplicate,
        // so swap self[r] and self[w] and advance r and w:
        //
        //                       r
        //     +---+---+---+---+---+---+
        //     | 0 | 1 | 2 | 1 | 3 | 3 |
        //     +---+---+---+---+---+---+
        //                   w
        //
        // Not a duplicate, repeat:
        //
        //                           r
        //     +---+---+---+---+---+---+
        //     | 0 | 1 | 2 | 3 | 1 | 3 |
        //     +---+---+---+---+---+---+
        //                       w
        //
        // Duplicate, advance r. End of slice. Split at w.

        let len = self.len();
        if len <= 1 {
            return (self, &mut []);
        }

        let ptr = self.as_mut_ptr();
        let mut next_read: usize = 1;
        let mut next_write: usize = 1;

        // SAFETY: the `while` condition guarantees `next_read` and `next_write`
        // are less than `len`, thus are inside `self`. `prev_ptr_write` points to
        // one element before `ptr_write`, but `next_write` starts at 1, so
        // `prev_ptr_write` is never less than 0 and is inside the slice.
        // This fulfils the requirements for dereferencing `ptr_read`, `prev_ptr_write`
        // and `ptr_write`, and for using `ptr.add(next_read)`, `ptr.add(next_write - 1)`
        // and `prev_ptr_write.offset(1)`.
        //
        // `next_write` is also incremented at most once per loop at most meaning
        // no element is skipped when it may need to be swapped.
        //
        // `ptr_read` and `prev_ptr_write` never point to the same element. This
        // is required for `&mut *ptr_read`, `&mut *prev_ptr_write` to be safe.
        // The explanation is simply that `next_read >= next_write` is always true,
        // thus `next_read > next_write - 1` is too.
        unsafe {
            // Avoid bounds checks by using raw pointers.
            while next_read < len {
                let ptr_read = ptr.add(next_read);
                let prev_ptr_write = ptr.add(next_write - 1);
                if !same_bucket(&mut *ptr_read, &mut *prev_ptr_write) {
                    if next_read != next_write {
                        let ptr_write = prev_ptr_write.offset(1);
                        mem::swap(&mut *ptr_read, &mut *ptr_write);
                    }
                    next_write += 1;
                }
                next_read += 1;
            }
        }

        self.split_at_mut(next_write)
    }

    /// Moves all but the first of consecutive elements to the end of the slice that resolve
    /// to the same key.
    ///
    /// Returns two slices. The first contains no consecutive repeated elements.
    /// The second contains all the duplicates in no specified order.
    ///
    /// If the slice is sorted, the first returned slice contains no duplicates.
    ///
    /// # Examples
    ///
    /// ```
    /// #![feature(slice_partition_dedup)]
    ///
    /// let mut slice = [10, 20, 21, 30, 30, 20, 11, 13];
    ///
    /// let (dedup, duplicates) = slice.partition_dedup_by_key(|i| *i / 10);
    ///
    /// assert_eq!(dedup, [10, 20, 30, 20, 11]);
    /// assert_eq!(duplicates, [21, 30, 13]);
    /// ```
    #[unstable(feature = "slice_partition_dedup", issue = "54279")]
    #[inline]
    pub fn partition_dedup_by_key<K, F>(&mut self, mut key: F) -> (&mut [T], &mut [T])
    where
        F: FnMut(&mut T) -> K,
        K: PartialEq,
    {
        self.partition_dedup_by(|a, b| key(a) == key(b))
    }

    /// Rotates the slice in-place such that the first `mid` elements of the
    /// slice move to the end while the last `self.len() - mid` elements move to
    /// the front. After calling `rotate_left`, the element previously at index
    /// `mid` will become the first element in the slice.
    ///
    /// # Panics
    ///
    /// This function will panic if `mid` is greater than the length of the
    /// slice. Note that `mid == self.len()` does _not_ panic and is a no-op
    /// rotation.
    ///
    /// # Complexity
    ///
    /// Takes linear (in `self.len()`) time.
    ///
    /// # Examples
    ///
    /// ```
    /// let mut a = ['a', 'b', 'c', 'd', 'e', 'f'];
    /// a.rotate_left(2);
    /// assert_eq!(a, ['c', 'd', 'e', 'f', 'a', 'b']);
    /// ```
    ///
    /// Rotating a subslice:
    ///
    /// ```
    /// let mut a = ['a', 'b', 'c', 'd', 'e', 'f'];
    /// a[1..5].rotate_left(1);
    /// assert_eq!(a, ['a', 'c', 'd', 'e', 'b', 'f']);
    /// ```
    #[stable(feature = "slice_rotate", since = "1.26.0")]
    pub fn rotate_left(&mut self, mid: usize) {
        assert!(mid <= self.len());
        let k = self.len() - mid;
        let p = self.as_mut_ptr();

        // SAFETY: The range `[p.add(mid) - mid, p.add(mid) + k)` is trivially
        // valid for reading and writing, as required by `ptr_rotate`.
        unsafe {
            rotate::ptr_rotate(mid, p.add(mid), k);
        }
    }

    /// Rotates the slice in-place such that the first `self.len() - k`
    /// elements of the slice move to the end while the last `k` elements move
    /// to the front. After calling `rotate_right`, the element previously at
    /// index `self.len() - k` will become the first element in the slice.
    ///
    /// # Panics
    ///
    /// This function will panic if `k` is greater than the length of the
    /// slice. Note that `k == self.len()` does _not_ panic and is a no-op
    /// rotation.
    ///
    /// # Complexity
    ///
    /// Takes linear (in `self.len()`) time.
    ///
    /// # Examples
    ///
    /// ```
    /// let mut a = ['a', 'b', 'c', 'd', 'e', 'f'];
    /// a.rotate_right(2);
    /// assert_eq!(a, ['e', 'f', 'a', 'b', 'c', 'd']);
    /// ```
    ///
    /// Rotate a subslice:
    ///
    /// ```
    /// let mut a = ['a', 'b', 'c', 'd', 'e', 'f'];
    /// a[1..5].rotate_right(1);
    /// assert_eq!(a, ['a', 'e', 'b', 'c', 'd', 'f']);
    /// ```
    #[stable(feature = "slice_rotate", since = "1.26.0")]
    pub fn rotate_right(&mut self, k: usize) {
        assert!(k <= self.len());
        let mid = self.len() - k;
        let p = self.as_mut_ptr();

        // SAFETY: The range `[p.add(mid) - mid, p.add(mid) + k)` is trivially
        // valid for reading and writing, as required by `ptr_rotate`.
        unsafe {
            rotate::ptr_rotate(mid, p.add(mid), k);
        }
    }

    /// Fills `self` with elements by cloning `value`.
    ///
    /// # Examples
    ///
    /// ```
    /// #![feature(slice_fill)]
    ///
    /// let mut buf = vec![0; 10];
    /// buf.fill(1);
    /// assert_eq!(buf, vec![1; 10]);
    /// ```
    #[unstable(feature = "slice_fill", issue = "70758")]
    pub fn fill(&mut self, value: T)
    where
        T: Clone,
    {
        if let Some((last, elems)) = self.split_last_mut() {
            for el in elems {
                el.clone_from(&value);
            }

            *last = value
        }
    }

    /// Copies the elements from `src` into `self`.
    ///
    /// The length of `src` must be the same as `self`.
    ///
    /// If `T` implements `Copy`, it can be more performant to use
    /// [`copy_from_slice`].
    ///
    /// # Panics
    ///
    /// This function will panic if the two slices have different lengths.
    ///
    /// # Examples
    ///
    /// Cloning two elements from a slice into another:
    ///
    /// ```
    /// let src = [1, 2, 3, 4];
    /// let mut dst = [0, 0];
    ///
    /// // Because the slices have to be the same length,
    /// // we slice the source slice from four elements
    /// // to two. It will panic if we don't do this.
    /// dst.clone_from_slice(&src[2..]);
    ///
    /// assert_eq!(src, [1, 2, 3, 4]);
    /// assert_eq!(dst, [3, 4]);
    /// ```
    ///
    /// Rust enforces that there can only be one mutable reference with no
    /// immutable references to a particular piece of data in a particular
    /// scope. Because of this, attempting to use `clone_from_slice` on a
    /// single slice will result in a compile failure:
    ///
    /// ```compile_fail
    /// let mut slice = [1, 2, 3, 4, 5];
    ///
    /// slice[..2].clone_from_slice(&slice[3..]); // compile fail!
    /// ```
    ///
    /// To work around this, we can use [`split_at_mut`] to create two distinct
    /// sub-slices from a slice:
    ///
    /// ```
    /// let mut slice = [1, 2, 3, 4, 5];
    ///
    /// {
    ///     let (left, right) = slice.split_at_mut(2);
    ///     left.clone_from_slice(&right[1..]);
    /// }
    ///
    /// assert_eq!(slice, [4, 5, 3, 4, 5]);
    /// ```
    ///
    /// [`copy_from_slice`]: #method.copy_from_slice
    /// [`split_at_mut`]: #method.split_at_mut
    #[stable(feature = "clone_from_slice", since = "1.7.0")]
    pub fn clone_from_slice(&mut self, src: &[T])
    where
        T: Clone,
    {
        assert!(self.len() == src.len(), "destination and source slices have different lengths");
        // NOTE: We need to explicitly slice them to the same length
        // for bounds checking to be elided, and the optimizer will
        // generate memcpy for simple cases (for example T = u8).
        let len = self.len();
        let src = &src[..len];
        for i in 0..len {
            self[i].clone_from(&src[i]);
        }
    }

    /// Copies all elements from `src` into `self`, using a memcpy.
    ///
    /// The length of `src` must be the same as `self`.
    ///
    /// If `T` does not implement `Copy`, use [`clone_from_slice`].
    ///
    /// # Panics
    ///
    /// This function will panic if the two slices have different lengths.
    ///
    /// # Examples
    ///
    /// Copying two elements from a slice into another:
    ///
    /// ```
    /// let src = [1, 2, 3, 4];
    /// let mut dst = [0, 0];
    ///
    /// // Because the slices have to be the same length,
    /// // we slice the source slice from four elements
    /// // to two. It will panic if we don't do this.
    /// dst.copy_from_slice(&src[2..]);
    ///
    /// assert_eq!(src, [1, 2, 3, 4]);
    /// assert_eq!(dst, [3, 4]);
    /// ```
    ///
    /// Rust enforces that there can only be one mutable reference with no
    /// immutable references to a particular piece of data in a particular
    /// scope. Because of this, attempting to use `copy_from_slice` on a
    /// single slice will result in a compile failure:
    ///
    /// ```compile_fail
    /// let mut slice = [1, 2, 3, 4, 5];
    ///
    /// slice[..2].copy_from_slice(&slice[3..]); // compile fail!
    /// ```
    ///
    /// To work around this, we can use [`split_at_mut`] to create two distinct
    /// sub-slices from a slice:
    ///
    /// ```
    /// let mut slice = [1, 2, 3, 4, 5];
    ///
    /// {
    ///     let (left, right) = slice.split_at_mut(2);
    ///     left.copy_from_slice(&right[1..]);
    /// }
    ///
    /// assert_eq!(slice, [4, 5, 3, 4, 5]);
    /// ```
    ///
    /// [`clone_from_slice`]: #method.clone_from_slice
    /// [`split_at_mut`]: #method.split_at_mut
    #[stable(feature = "copy_from_slice", since = "1.9.0")]
    pub fn copy_from_slice(&mut self, src: &[T])
    where
        T: Copy,
    {
        // The panic code path was put into a cold function to not bloat the
        // call site.
        #[inline(never)]
        #[cold]
        #[track_caller]
        fn len_mismatch_fail(dst_len: usize, src_len: usize) -> ! {
            panic!(
                "source slice length ({}) does not match destination slice length ({})",
                src_len, dst_len,
            );
        }

        if self.len() != src.len() {
            len_mismatch_fail(self.len(), src.len());
        }

        // SAFETY: `self` is valid for `self.len()` elements by definition, and `src` was
        // checked to have the same length. The slices cannot overlap because
        // mutable references are exclusive.
        unsafe {
            ptr::copy_nonoverlapping(src.as_ptr(), self.as_mut_ptr(), self.len());
        }
    }

    /// Copies elements from one part of the slice to another part of itself,
    /// using a memmove.
    ///
    /// `src` is the range within `self` to copy from. `dest` is the starting
    /// index of the range within `self` to copy to, which will have the same
    /// length as `src`. The two ranges may overlap. The ends of the two ranges
    /// must be less than or equal to `self.len()`.
    ///
    /// # Panics
    ///
    /// This function will panic if either range exceeds the end of the slice,
    /// or if the end of `src` is before the start.
    ///
    /// # Examples
    ///
    /// Copying four bytes within a slice:
    ///
    /// ```
    /// let mut bytes = *b"Hello, World!";
    ///
    /// bytes.copy_within(1..5, 8);
    ///
    /// assert_eq!(&bytes, b"Hello, Wello!");
    /// ```
    #[stable(feature = "copy_within", since = "1.37.0")]
    #[track_caller]
    pub fn copy_within<R: RangeBounds<usize>>(&mut self, src: R, dest: usize)
    where
        T: Copy,
    {
        let Range { start: src_start, end: src_end } = src.assert_len(self.len());
        let count = src_end - src_start;
        assert!(dest <= self.len() - count, "dest is out of bounds");
        // SAFETY: the conditions for `ptr::copy` have all been checked above,
        // as have those for `ptr::add`.
        unsafe {
            ptr::copy(self.as_ptr().add(src_start), self.as_mut_ptr().add(dest), count);
        }
    }

    /// Swaps all elements in `self` with those in `other`.
    ///
    /// The length of `other` must be the same as `self`.
    ///
    /// # Panics
    ///
    /// This function will panic if the two slices have different lengths.
    ///
    /// # Example
    ///
    /// Swapping two elements across slices:
    ///
    /// ```
    /// let mut slice1 = [0, 0];
    /// let mut slice2 = [1, 2, 3, 4];
    ///
    /// slice1.swap_with_slice(&mut slice2[2..]);
    ///
    /// assert_eq!(slice1, [3, 4]);
    /// assert_eq!(slice2, [1, 2, 0, 0]);
    /// ```
    ///
    /// Rust enforces that there can only be one mutable reference to a
    /// particular piece of data in a particular scope. Because of this,
    /// attempting to use `swap_with_slice` on a single slice will result in
    /// a compile failure:
    ///
    /// ```compile_fail
    /// let mut slice = [1, 2, 3, 4, 5];
    /// slice[..2].swap_with_slice(&mut slice[3..]); // compile fail!
    /// ```
    ///
    /// To work around this, we can use [`split_at_mut`] to create two distinct
    /// mutable sub-slices from a slice:
    ///
    /// ```
    /// let mut slice = [1, 2, 3, 4, 5];
    ///
    /// {
    ///     let (left, right) = slice.split_at_mut(2);
    ///     left.swap_with_slice(&mut right[1..]);
    /// }
    ///
    /// assert_eq!(slice, [4, 5, 3, 1, 2]);
    /// ```
    ///
    /// [`split_at_mut`]: #method.split_at_mut
    #[stable(feature = "swap_with_slice", since = "1.27.0")]
    pub fn swap_with_slice(&mut self, other: &mut [T]) {
        assert!(self.len() == other.len(), "destination and source slices have different lengths");
        // SAFETY: `self` is valid for `self.len()` elements by definition, and `src` was
        // checked to have the same length. The slices cannot overlap because
        // mutable references are exclusive.
        unsafe {
            ptr::swap_nonoverlapping(self.as_mut_ptr(), other.as_mut_ptr(), self.len());
        }
    }

    /// Function to calculate lengths of the middle and trailing slice for `align_to{,_mut}`.
    fn align_to_offsets<U>(&self) -> (usize, usize) {
        // What we gonna do about `rest` is figure out what multiple of `U`s we can put in a
        // lowest number of `T`s. And how many `T`s we need for each such "multiple".
        //
        // Consider for example T=u8 U=u16. Then we can put 1 U in 2 Ts. Simple. Now, consider
        // for example a case where size_of::<T> = 16, size_of::<U> = 24. We can put 2 Us in
        // place of every 3 Ts in the `rest` slice. A bit more complicated.
        //
        // Formula to calculate this is:
        //
        // Us = lcm(size_of::<T>, size_of::<U>) / size_of::<U>
        // Ts = lcm(size_of::<T>, size_of::<U>) / size_of::<T>
        //
        // Expanded and simplified:
        //
        // Us = size_of::<T> / gcd(size_of::<T>, size_of::<U>)
        // Ts = size_of::<U> / gcd(size_of::<T>, size_of::<U>)
        //
        // Luckily since all this is constant-evaluated... performance here matters not!
        #[inline]
        fn gcd(a: usize, b: usize) -> usize {
            use crate::intrinsics;
            // iterative stein’s algorithm
            // We should still make this `const fn` (and revert to recursive algorithm if we do)
            // because relying on llvm to consteval all this is… well, it makes me uncomfortable.

            // SAFETY: `a` and `b` are checked to be non-zero values.
            let (ctz_a, mut ctz_b) = unsafe {
                if a == 0 {
                    return b;
                }
                if b == 0 {
                    return a;
                }
                (intrinsics::cttz_nonzero(a), intrinsics::cttz_nonzero(b))
            };
            let k = ctz_a.min(ctz_b);
            let mut a = a >> ctz_a;
            let mut b = b;
            loop {
                // remove all factors of 2 from b
                b >>= ctz_b;
                if a > b {
                    mem::swap(&mut a, &mut b);
                }
                b = b - a;
                // SAFETY: `b` is checked to be non-zero.
                unsafe {
                    if b == 0 {
                        break;
                    }
                    ctz_b = intrinsics::cttz_nonzero(b);
                }
            }
            a << k
        }
        let gcd: usize = gcd(mem::size_of::<T>(), mem::size_of::<U>());
        let ts: usize = mem::size_of::<U>() / gcd;
        let us: usize = mem::size_of::<T>() / gcd;

        // Armed with this knowledge, we can find how many `U`s we can fit!
        let us_len = self.len() / ts * us;
        // And how many `T`s will be in the trailing slice!
        let ts_len = self.len() % ts;
        (us_len, ts_len)
    }

    /// Transmute the slice to a slice of another type, ensuring alignment of the types is
    /// maintained.
    ///
    /// This method splits the slice into three distinct slices: prefix, correctly aligned middle
    /// slice of a new type, and the suffix slice. The method may make the middle slice the greatest
    /// length possible for a given type and input slice, but only your algorithm's performance
    /// should depend on that, not its correctness. It is permissible for all of the input data to
    /// be returned as the prefix or suffix slice.
    ///
    /// This method has no purpose when either input element `T` or output element `U` are
    /// zero-sized and will return the original slice without splitting anything.
    ///
    /// # Safety
    ///
    /// This method is essentially a `transmute` with respect to the elements in the returned
    /// middle slice, so all the usual caveats pertaining to `transmute::<T, U>` also apply here.
    ///
    /// # Examples
    ///
    /// Basic usage:
    ///
    /// ```
    /// unsafe {
    ///     let bytes: [u8; 7] = [1, 2, 3, 4, 5, 6, 7];
    ///     let (prefix, shorts, suffix) = bytes.align_to::<u16>();
    ///     // less_efficient_algorithm_for_bytes(prefix);
    ///     // more_efficient_algorithm_for_aligned_shorts(shorts);
    ///     // less_efficient_algorithm_for_bytes(suffix);
    /// }
    /// ```
    #[stable(feature = "slice_align_to", since = "1.30.0")]
    pub unsafe fn align_to<U>(&self) -> (&[T], &[U], &[T]) {
        // Note that most of this function will be constant-evaluated,
        if mem::size_of::<U>() == 0 || mem::size_of::<T>() == 0 {
            // handle ZSTs specially, which is – don't handle them at all.
            return (self, &[], &[]);
        }

        // First, find at what point do we split between the first and 2nd slice. Easy with
        // ptr.align_offset.
        let ptr = self.as_ptr();
        // SAFETY: See the `align_to_mut` method for the detailed safety comment.
        let offset = unsafe { crate::ptr::align_offset(ptr, mem::align_of::<U>()) };
        if offset > self.len() {
            (self, &[], &[])
        } else {
            let (left, rest) = self.split_at(offset);
            let (us_len, ts_len) = rest.align_to_offsets::<U>();
            // SAFETY: now `rest` is definitely aligned, so `from_raw_parts` below is okay,
            // since the caller guarantees that we can transmute `T` to `U` safely.
            unsafe {
                (
                    left,
                    from_raw_parts(rest.as_ptr() as *const U, us_len),
                    from_raw_parts(rest.as_ptr().add(rest.len() - ts_len), ts_len),
                )
            }
        }
    }

    /// Transmute the slice to a slice of another type, ensuring alignment of the types is
    /// maintained.
    ///
    /// This method splits the slice into three distinct slices: prefix, correctly aligned middle
    /// slice of a new type, and the suffix slice. The method may make the middle slice the greatest
    /// length possible for a given type and input slice, but only your algorithm's performance
    /// should depend on that, not its correctness. It is permissible for all of the input data to
    /// be returned as the prefix or suffix slice.
    ///
    /// This method has no purpose when either input element `T` or output element `U` are
    /// zero-sized and will return the original slice without splitting anything.
    ///
    /// # Safety
    ///
    /// This method is essentially a `transmute` with respect to the elements in the returned
    /// middle slice, so all the usual caveats pertaining to `transmute::<T, U>` also apply here.
    ///
    /// # Examples
    ///
    /// Basic usage:
    ///
    /// ```
    /// unsafe {
    ///     let mut bytes: [u8; 7] = [1, 2, 3, 4, 5, 6, 7];
    ///     let (prefix, shorts, suffix) = bytes.align_to_mut::<u16>();
    ///     // less_efficient_algorithm_for_bytes(prefix);
    ///     // more_efficient_algorithm_for_aligned_shorts(shorts);
    ///     // less_efficient_algorithm_for_bytes(suffix);
    /// }
    /// ```
    #[stable(feature = "slice_align_to", since = "1.30.0")]
    pub unsafe fn align_to_mut<U>(&mut self) -> (&mut [T], &mut [U], &mut [T]) {
        // Note that most of this function will be constant-evaluated,
        if mem::size_of::<U>() == 0 || mem::size_of::<T>() == 0 {
            // handle ZSTs specially, which is – don't handle them at all.
            return (self, &mut [], &mut []);
        }

        // First, find at what point do we split between the first and 2nd slice. Easy with
        // ptr.align_offset.
        let ptr = self.as_ptr();
        // SAFETY: Here we are ensuring we will use aligned pointers for U for the
        // rest of the method. This is done by passing a pointer to &[T] with an
        // alignment targeted for U.
        // `crate::ptr::align_offset` is called with a correctly aligned and
        // valid pointer `ptr` (it comes from a reference to `self`) and with
        // a size that is a power of two (since it comes from the alignement for U),
        // satisfying its safety constraints.
        let offset = unsafe { crate::ptr::align_offset(ptr, mem::align_of::<U>()) };
        if offset > self.len() {
            (self, &mut [], &mut [])
        } else {
            let (left, rest) = self.split_at_mut(offset);
            let (us_len, ts_len) = rest.align_to_offsets::<U>();
            let rest_len = rest.len();
            let mut_ptr = rest.as_mut_ptr();
            // We can't use `rest` again after this, that would invalidate its alias `mut_ptr`!
            // SAFETY: see comments for `align_to`.
            unsafe {
                (
                    left,
                    from_raw_parts_mut(mut_ptr as *mut U, us_len),
                    from_raw_parts_mut(mut_ptr.add(rest_len - ts_len), ts_len),
                )
            }
        }
    }

    /// Checks if the elements of this slice are sorted.
    ///
    /// That is, for each element `a` and its following element `b`, `a <= b` must hold. If the
    /// slice yields exactly zero or one element, `true` is returned.
    ///
    /// Note that if `Self::Item` is only `PartialOrd`, but not `Ord`, the above definition
    /// implies that this function returns `false` if any two consecutive items are not
    /// comparable.
    ///
    /// # Examples
    ///
    /// ```
    /// #![feature(is_sorted)]
    /// let empty: [i32; 0] = [];
    ///
    /// assert!([1, 2, 2, 9].is_sorted());
    /// assert!(![1, 3, 2, 4].is_sorted());
    /// assert!([0].is_sorted());
    /// assert!(empty.is_sorted());
    /// assert!(![0.0, 1.0, f32::NAN].is_sorted());
    /// ```
    #[inline]
    #[unstable(feature = "is_sorted", reason = "new API", issue = "53485")]
    pub fn is_sorted(&self) -> bool
    where
        T: PartialOrd,
    {
        self.is_sorted_by(|a, b| a.partial_cmp(b))
    }

    /// Checks if the elements of this slice are sorted using the given comparator function.
    ///
    /// Instead of using `PartialOrd::partial_cmp`, this function uses the given `compare`
    /// function to determine the ordering of two elements. Apart from that, it's equivalent to
    /// [`is_sorted`]; see its documentation for more information.
    ///
    /// [`is_sorted`]: #method.is_sorted
    #[unstable(feature = "is_sorted", reason = "new API", issue = "53485")]
    pub fn is_sorted_by<F>(&self, mut compare: F) -> bool
    where
        F: FnMut(&T, &T) -> Option<Ordering>,
    {
        self.iter().is_sorted_by(|a, b| compare(*a, *b))
    }

    /// Checks if the elements of this slice are sorted using the given key extraction function.
    ///
    /// Instead of comparing the slice's elements directly, this function compares the keys of the
    /// elements, as determined by `f`. Apart from that, it's equivalent to [`is_sorted`]; see its
    /// documentation for more information.
    ///
    /// [`is_sorted`]: #method.is_sorted
    ///
    /// # Examples
    ///
    /// ```
    /// #![feature(is_sorted)]
    ///
    /// assert!(["c", "bb", "aaa"].is_sorted_by_key(|s| s.len()));
    /// assert!(![-2i32, -1, 0, 3].is_sorted_by_key(|n| n.abs()));
    /// ```
    #[inline]
    #[unstable(feature = "is_sorted", reason = "new API", issue = "53485")]
    pub fn is_sorted_by_key<F, K>(&self, f: F) -> bool
    where
        F: FnMut(&T) -> K,
        K: PartialOrd,
    {
        self.iter().is_sorted_by_key(f)
    }

    /// Returns the index of the partition point according to the given predicate
    /// (the index of the first element of the second partition).
    ///
    /// The slice is assumed to be partitioned according to the given predicate.
    /// This means that all elements for which the predicate returns true are at the start of the slice
    /// and all elements for which the predicate returns false are at the end.
    /// For example, [7, 15, 3, 5, 4, 12, 6] is a partitioned under the predicate x % 2 != 0
    /// (all odd numbers are at the start, all even at the end).
    ///
    /// If this slice is not partitioned, the returned result is unspecified and meaningless,
    /// as this method performs a kind of binary search.
    ///
    /// # Examples
    ///
    /// ```
    /// #![feature(partition_point)]
    ///
    /// let v = [1, 2, 3, 3, 5, 6, 7];
    /// let i = v.partition_point(|&x| x < 5);
    ///
    /// assert_eq!(i, 4);
    /// assert!(v[..i].iter().all(|&x| x < 5));
    /// assert!(v[i..].iter().all(|&x| !(x < 5)));
    /// ```
    #[unstable(feature = "partition_point", reason = "new API", issue = "73831")]
    pub fn partition_point<P>(&self, mut pred: P) -> usize
    where
        P: FnMut(&T) -> bool,
    {
        let mut left = 0;
        let mut right = self.len();

        while left != right {
            let mid = left + (right - left) / 2;
            // SAFETY: When `left < right`, `left <= mid < right`.
            // Therefore `left` always increases and `right` always decreases,
            // and either of them is selected. In both cases `left <= right` is
            // satisfied. Therefore if `left < right` in a step, `left <= right`
            // is satisfied in the next step. Therefore as long as `left != right`,
            // `0 <= left < right <= len` is satisfied and if this case
            // `0 <= mid < len` is satisfied too.
            let value = unsafe { self.get_unchecked(mid) };
            if pred(value) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }

        left
    }
}

#[stable(feature = "rust1", since = "1.0.0")]
impl<T> Default for &[T] {
    /// Creates an empty slice.
    fn default() -> Self {
        &[]
    }
}

#[stable(feature = "mut_slice_default", since = "1.5.0")]
impl<T> Default for &mut [T] {
    /// Creates a mutable empty slice.
    fn default() -> Self {
        &mut []
    }
}
