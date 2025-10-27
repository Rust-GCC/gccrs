/// Extracts the successful type of a `Poll<T>`.
///
/// This macro bakes in propagation of `Pending` signals by returning early.
///
/// # Examples
///
/// ```
/// #![feature(ready_macro)]
///
/// use core::task::{ready, Context, Poll};
/// use core::future::{self, Future};
/// use core::pin::Pin;
///
/// pub fn do_poll(cx: &mut Context<'_>) -> Poll<()> {
///     let mut fut = future::ready(42);
///     let fut = Pin::new(&mut fut);
///
///     let num = ready!(fut.poll(cx));
///     # drop(num);
///     // ... use num
///
///     Poll::Ready(())
/// }
/// ```
///
/// The `ready!` call expands to:
///
/// ```
/// # #![feature(ready_macro)]
/// #
/// # use core::task::{Context, Poll};
/// # use core::future::{self, Future};
/// # use core::pin::Pin;
/// #
/// # pub fn do_poll(cx: &mut Context<'_>) -> Poll<()> {
///     # let mut fut = future::ready(42);
///     # let fut = Pin::new(&mut fut);
///     #
/// let num = match fut.poll(cx) {
///     Poll::Ready(t) => t,
///     Poll::Pending => return Poll::Pending,
/// };
///     # drop(num);
///     # // ... use num
///     #
///     # Poll::Ready(())
/// # }
/// ```
#[unstable(feature = "ready_macro", issue = "70922")]
#[rustc_macro_transparency = "semitransparent"]
pub macro ready($e:expr) {
    match $e {
        $crate::task::Poll::Ready(t) => t,
        $crate::task::Poll::Pending => {
            return $crate::task::Poll::Pending;
        }
    }
}
