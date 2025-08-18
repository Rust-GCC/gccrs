/// An iterator that always continues to yield `None` when exhausted.
///
/// Calling next on a fused iterator that has returned `None` once is guaranteed
/// to return [`None`] again. This trait should be implemented by all iterators
/// that behave this way because it allows optimizing [`Iterator::fuse()`].
///
/// Note: In general, you should not use `FusedIterator` in generic bounds if
/// you need a fused iterator. Instead, you should just call [`Iterator::fuse()`]
/// on the iterator. If the iterator is already fused, the additional [`Fuse`]
/// wrapper will be a no-op with no performance penalty.
///
/// [`Fuse`]: crate::iter::Fuse
#[stable(feature = "fused", since = "1.26.0")]
#[rustc_unsafe_specialization_marker]
pub trait FusedIterator: Iterator {}

#[stable(feature = "fused", since = "1.26.0")]
impl<I: FusedIterator + ?Sized> FusedIterator for &mut I {}

/// An iterator that reports an accurate length using size_hint.
///
/// The iterator reports a size hint where it is either exact
/// (lower bound is equal to upper bound), or the upper bound is [`None`].
/// The upper bound must only be [`None`] if the actual iterator length is
/// larger than [`usize::MAX`]. In that case, the lower bound must be
/// [`usize::MAX`], resulting in a [`Iterator::size_hint()`] of
/// `(usize::MAX, None)`.
///
/// The iterator must produce exactly the number of elements it reported
/// or diverge before reaching the end.
///
/// # Safety
///
/// This trait must only be implemented when the contract is upheld. Consumers
/// of this trait must inspect [`Iterator::size_hint()`]’s upper bound.
///
/// [`usize::MAX`]: crate::usize::MAX
#[unstable(feature = "trusted_len", issue = "37572")]
#[rustc_unsafe_specialization_marker]
pub unsafe trait TrustedLen: Iterator {}

#[unstable(feature = "trusted_len", issue = "37572")]
unsafe impl<I: TrustedLen + ?Sized> TrustedLen for &mut I {}

/// An iterator that when yielding an item will have taken at least one element
/// from its underlying [`SourceIter`].
///
/// Calling [`next()`] guarantees that at least one value of the iterator's underlying source
/// has been moved out and the result of the iterator chain could be inserted in its place,
/// assuming structural constraints of the source allow such an insertion.
/// In other words this trait indicates that an iterator pipeline can be collected in place.
///
/// [`SourceIter`]: crate::iter::SourceIter
/// [`next()`]: Iterator::next
#[unstable(issue = "none", feature = "inplace_iteration")]
pub unsafe trait InPlaceIterable: Iterator {}
