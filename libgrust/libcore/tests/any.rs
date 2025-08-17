use core::any::*;

#[derive(PartialEq, Debug)]
struct Test;

static TEST: &'static str = "Test";

#[test]
fn any_referenced() {
    let (a, b, c) = (&5 as &dyn Any, &TEST as &dyn Any, &Test as &dyn Any);

    assert!(a.is::<i32>());
    assert!(!b.is::<i32>());
    assert!(!c.is::<i32>());

    assert!(!a.is::<&'static str>());
    assert!(b.is::<&'static str>());
    assert!(!c.is::<&'static str>());

    assert!(!a.is::<Test>());
    assert!(!b.is::<Test>());
    assert!(c.is::<Test>());
}

#[test]
fn any_owning() {
    let (a, b, c) =
        (box 5_usize as Box<dyn Any>, box TEST as Box<dyn Any>, box Test as Box<dyn Any>);

    assert!(a.is::<usize>());
    assert!(!b.is::<usize>());
    assert!(!c.is::<usize>());

    assert!(!a.is::<&'static str>());
    assert!(b.is::<&'static str>());
    assert!(!c.is::<&'static str>());

    assert!(!a.is::<Test>());
    assert!(!b.is::<Test>());
    assert!(c.is::<Test>());
}

#[test]
fn any_downcast_ref() {
    let a = &5_usize as &dyn Any;

    match a.downcast_ref::<usize>() {
        Some(&5) => {}
        x => panic!("Unexpected value {:?}", x),
    }

    match a.downcast_ref::<Test>() {
        None => {}
        x => panic!("Unexpected value {:?}", x),
    }
}

#[test]
fn any_downcast_mut() {
    let mut a = 5_usize;
    let mut b: Box<_> = box 7_usize;

    let a_r = &mut a as &mut dyn Any;
    let tmp: &mut usize = &mut *b;
    let b_r = tmp as &mut dyn Any;

    match a_r.downcast_mut::<usize>() {
        Some(x) => {
            assert_eq!(*x, 5);
            *x = 612;
        }
        x => panic!("Unexpected value {:?}", x),
    }

    match b_r.downcast_mut::<usize>() {
        Some(x) => {
            assert_eq!(*x, 7);
            *x = 413;
        }
        x => panic!("Unexpected value {:?}", x),
    }

    match a_r.downcast_mut::<Test>() {
        None => (),
        x => panic!("Unexpected value {:?}", x),
    }

    match b_r.downcast_mut::<Test>() {
        None => (),
        x => panic!("Unexpected value {:?}", x),
    }

    match a_r.downcast_mut::<usize>() {
        Some(&mut 612) => {}
        x => panic!("Unexpected value {:?}", x),
    }

    match b_r.downcast_mut::<usize>() {
        Some(&mut 413) => {}
        x => panic!("Unexpected value {:?}", x),
    }
}

#[test]
fn any_fixed_vec() {
    let test = [0_usize; 8];
    let test = &test as &dyn Any;
    assert!(test.is::<[usize; 8]>());
    assert!(!test.is::<[usize; 10]>());
}

#[test]
fn any_unsized() {
    fn is_any<T: Any + ?Sized>() {}
    is_any::<[i32]>();
}
