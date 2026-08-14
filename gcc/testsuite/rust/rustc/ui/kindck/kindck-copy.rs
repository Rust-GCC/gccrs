// Test which of the builtin types are considered POD.

use std::rc::Rc;

fn assert_copy<T:Copy>() { }

trait Dummy { }

#[derive(Copy, Clone)]
struct MyStruct {
    x: isize,
    y: isize,
}

struct MyNoncopyStruct {
    x: Box<char>,
}

fn test<'a,T,U:Copy>(_: &'a isize) {
    // lifetime pointers are ok...
    assert_copy::<&'static isize>();
    assert_copy::<&'a isize>();
    assert_copy::<&'a str>();
    assert_copy::<&'a [isize]>();

    // ...unless they are mutable
    assert_copy::<&'static mut isize>(); // { dg-error ".E0277." "" { target *-*-* } }
    assert_copy::<&'a mut isize>();  // { dg-error ".E0277." "" { target *-*-* } }

    // boxes are not ok
    assert_copy::<Box<isize>>();   // { dg-error ".E0277." "" { target *-*-* } }
    assert_copy::<String>();   // { dg-error ".E0277." "" { target *-*-* } }
    assert_copy::<Vec<isize> >(); // { dg-error ".E0277." "" { target *-*-* } }
    assert_copy::<Box<&'a mut isize>>(); // { dg-error ".E0277." "" { target *-*-* } }

    // borrowed object types are generally ok
    assert_copy::<&'a dyn Dummy>();
    assert_copy::<&'a (dyn Dummy + Send)>();
    assert_copy::<&'static (dyn Dummy + Send)>();

    // owned object types are not ok
    assert_copy::<Box<dyn Dummy>>(); // { dg-error ".E0277." "" { target *-*-* } }
    assert_copy::<Box<dyn Dummy + Send>>(); // { dg-error ".E0277." "" { target *-*-* } }

    // mutable object types are not ok
    assert_copy::<&'a mut (dyn Dummy + Send)>();  // { dg-error ".E0277." "" { target *-*-* } }

    // unsafe ptrs are ok
    assert_copy::<*const isize>();
    assert_copy::<*const &'a mut isize>();

    // regular old ints and such are ok
    assert_copy::<isize>();
    assert_copy::<bool>();
    assert_copy::<()>();

    // tuples are ok
    assert_copy::<(isize,isize)>();

    // structs of POD are ok
    assert_copy::<MyStruct>();

    // structs containing non-POD are not ok
    assert_copy::<MyNoncopyStruct>(); // { dg-error ".E0277." "" { target *-*-* } }

    // ref counted types are not ok
    assert_copy::<Rc<isize>>();   // { dg-error ".E0277." "" { target *-*-* } }
}

pub fn main() {
}

