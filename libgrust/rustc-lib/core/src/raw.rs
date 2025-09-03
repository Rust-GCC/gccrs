#![allow(missing_docs)]
#![unstable(feature = "raw", issue = "27751")]

//! Contains struct definitions for the layout of compiler built-in types.
//!
//! They can be used as targets of transmutes in unsafe code for manipulating
//! the raw representations directly.
//!
//! Their definition should always match the ABI defined in
//! `rustc_middle::ty::layout`.

/// The representation of a trait object like `&dyn SomeTrait`.
///
/// This struct has the same layout as types like `&dyn SomeTrait` and
/// `Box<dyn AnotherTrait>`.
///
/// `TraitObject` is guaranteed to match layouts, but it is not the
/// type of trait objects (e.g., the fields are not directly accessible
/// on a `&dyn SomeTrait`) nor does it control that layout (changing the
/// definition will not change the layout of a `&dyn SomeTrait`). It is
/// only designed to be used by unsafe code that needs to manipulate
/// the low-level details.
///
/// There is no way to refer to all trait objects generically, so the only
/// way to create values of this type is with functions like
/// [`std::mem::transmute`][transmute]. Similarly, the only way to create a true
/// trait object from a `TraitObject` value is with `transmute`.
///
/// [transmute]: crate::intrinsics::transmute
///
/// Synthesizing a trait object with mismatched types—one where the
/// vtable does not correspond to the type of the value to which the
/// data pointer points—is highly likely to lead to undefined
/// behavior.
///
/// # Examples
///
/// ```
/// #![feature(raw)]
///
/// use std::{mem, raw};
///
/// // an example trait
/// trait Foo {
///     fn bar(&self) -> i32;
/// }
///
/// impl Foo for i32 {
///     fn bar(&self) -> i32 {
///          *self + 1
///     }
/// }
///
/// let value: i32 = 123;
///
/// // let the compiler make a trait object
/// let object: &dyn Foo = &value;
///
/// // look at the raw representation
/// let raw_object: raw::TraitObject = unsafe { mem::transmute(object) };
///
/// // the data pointer is the address of `value`
/// assert_eq!(raw_object.data as *const i32, &value as *const _);
///
/// let other_value: i32 = 456;
///
/// // construct a new object, pointing to a different `i32`, being
/// // careful to use the `i32` vtable from `object`
/// let synthesized: &dyn Foo = unsafe {
///      mem::transmute(raw::TraitObject {
///          data: &other_value as *const _ as *mut (),
///          vtable: raw_object.vtable,
///      })
/// };
///
/// // it should work just as if we had constructed a trait object out of
/// // `other_value` directly
/// assert_eq!(synthesized.bar(), 457);
/// ```
#[repr(C)]
#[derive(Copy, Clone)]
#[allow(missing_debug_implementations)]
pub struct TraitObject {
    pub data: *mut (),
    pub vtable: *mut (),
}
