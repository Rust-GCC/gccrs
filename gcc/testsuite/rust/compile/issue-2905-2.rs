#![feature(intrinsics)]
#![feature(lang_items)]

#[lang = "sized"]
trait Sized {}

extern "rust-intrinsic" {
    fn transmute<T, U>(_: T) -> U;
    fn offset<T>(src: *const T, offset: isize) -> *const T;
}

pub mod core {
    pub mod marker {
        #[lang = "phantom_data"]
        pub struct PhantomData<T>;
    }

    pub mod slice {
        use core::marker::PhantomData;
        use core::option::Option;

        impl<T> core::iter::IntoIterator for &[T] {
            type Item = &T;
            type IntoIter = Weird<T>;

            fn into_iter(self) -> Weird<T> {
                self.iter()
            }
        }

        pub struct Weird<T> {
            ptr: *const T, // should be NonNull<T> but here it does not matter
            // { dg-warning "field is never read" "" { target *-*-* } .-1 }
            end: *const T,
            // { dg-warning "field is never read" "" { target *-*-* } .-1 }
            _marker: PhantomData<&T>,
            // { dg-warning "field is never read" "" { target *-*-* } .-1 }
        }

        impl<T> Weird<T> {
            pub(super) fn new(slice: &[T]) -> Self {
                // { dg-warning "associated function is never used" "" { target *-*-* } .-1 }
                let ptr = slice.as_ptr();
                // SAFETY: Similar to `IterMut::new`.
                unsafe {
                    // should be: ptr.add(slice.len())
                    let end = transmute::<*const T, usize>(ptr) + slice.len(); // TODO(Arthur): Missing `* size_of::<T>()`?
                    let end = transmute::<usize, *const T>(end);

                    Self {
                        ptr,
                        end,
                        _marker: PhantomData,
                    }
                }
            }

            fn is_empty(&self) -> bool {
                // { dg-warning "associated function is never used" "" { target *-*-* } .-1 }
                self.ptr == self.end
            }

            fn next_unchecked(&mut self) -> *const T {
                // { dg-warning "associated function is never used" "" { target *-*-* } .-1 }
                let old = self.ptr;

                self.ptr = unsafe { offset(self.ptr, 1) };

                old
            }
        }

        trait Foo {}

        impl<T> Foo for Weird<T> {}

        // impl<T> core::iter::Iterator for Iter<T> {
        //     type Item = &T;

        //     fn next(&mut self) -> Option<&T> {
        //         if self.is_empty() {
        //             Option::None
        //         } else {
        //             Option::Some(&*self.next_unchecked())
        //         }
        //     }
        // }

        union Repr<T> {
            pub(crate) rust: *const [T],
            rust_mut: *mut [T],
            pub(crate) raw: FatPtr<T>,
        }

        struct FatPtr<T> {
            // { dg-warning "struct is never constructed" "" { target *-*-* } .-1 }
            data: *const T,
            pub(crate) len: usize,
        }

        impl<T> [T] {
            pub fn iter(&self) -> Weird<T> {
                Weird::new(self)
            }

            pub fn as_ptr(&self) -> *const T {
                self as *const [T] as *const T
            }

            pub fn len(&self) -> usize {
                unsafe { Repr { rust: self }.raw.len }
            }
        }
    }

    pub mod iter {
        use option::Option;

        pub trait IntoIterator {
            type Item;

            type IntoIter: Iterator<Item = Self::Item>;

            fn into_iter(self) -> Self::IntoIter;
        }

        pub trait Iterator {
            type Item;

            fn next(&mut self) -> Option<Self::Item>;
        }
    }

    pub mod option {
        pub enum Option<T> {
            Some(T),
            None,
        }
    }
}

fn main() {}
