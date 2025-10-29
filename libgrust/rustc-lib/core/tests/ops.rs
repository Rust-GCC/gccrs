use core::ops::{Bound, Range, RangeFrom, RangeFull, RangeInclusive, RangeTo};

// Test the Range structs and syntax.

#[test]
fn test_range() {
    let r = Range { start: 2, end: 10 };
    let mut count = 0;
    for (i, ri) in r.enumerate() {
        assert_eq!(ri, i + 2);
        assert!(ri >= 2 && ri < 10);
        count += 1;
    }
    assert_eq!(count, 8);
}

#[test]
fn test_range_from() {
    let r = RangeFrom { start: 2 };
    let mut count = 0;
    for (i, ri) in r.take(10).enumerate() {
        assert_eq!(ri, i + 2);
        assert!(ri >= 2 && ri < 12);
        count += 1;
    }
    assert_eq!(count, 10);
}

#[test]
fn test_range_to() {
    // Not much to test.
    let _ = RangeTo { end: 42 };
}

#[test]
fn test_full_range() {
    // Not much to test.
    let _ = RangeFull;
}

#[test]
fn test_range_inclusive() {
    let mut r = RangeInclusive::new(1i8, 2);
    assert_eq!(r.next(), Some(1));
    assert_eq!(r.next(), Some(2));
    assert_eq!(r.next(), None);

    r = RangeInclusive::new(127i8, 127);
    assert_eq!(r.next(), Some(127));
    assert_eq!(r.next(), None);

    r = RangeInclusive::new(-128i8, -128);
    assert_eq!(r.next_back(), Some(-128));
    assert_eq!(r.next_back(), None);

    // degenerate
    r = RangeInclusive::new(1, -1);
    assert_eq!(r.size_hint(), (0, Some(0)));
    assert_eq!(r.next(), None);
}

#[test]
fn test_range_is_empty() {
    assert!(!(0.0..10.0).is_empty());
    assert!((-0.0..0.0).is_empty());
    assert!((10.0..0.0).is_empty());

    assert!(!(f32::NEG_INFINITY..f32::INFINITY).is_empty());
    assert!((f32::EPSILON..f32::NAN).is_empty());
    assert!((f32::NAN..f32::EPSILON).is_empty());
    assert!((f32::NAN..f32::NAN).is_empty());

    assert!(!(0.0..=10.0).is_empty());
    assert!(!(-0.0..=0.0).is_empty());
    assert!((10.0..=0.0).is_empty());

    assert!(!(f32::NEG_INFINITY..=f32::INFINITY).is_empty());
    assert!((f32::EPSILON..=f32::NAN).is_empty());
    assert!((f32::NAN..=f32::EPSILON).is_empty());
    assert!((f32::NAN..=f32::NAN).is_empty());
}

#[test]
fn test_bound_cloned_unbounded() {
    assert_eq!(Bound::<&u32>::Unbounded.cloned(), Bound::Unbounded);
}

#[test]
fn test_bound_cloned_included() {
    assert_eq!(Bound::Included(&3).cloned(), Bound::Included(3));
}

#[test]
fn test_bound_cloned_excluded() {
    assert_eq!(Bound::Excluded(&3).cloned(), Bound::Excluded(3));
}

#[test]
#[allow(unused_comparisons)]
#[allow(unused_mut)]
fn test_range_syntax() {
    let mut count = 0;
    for i in 0_usize..10 {
        assert!(i >= 0 && i < 10);
        count += i;
    }
    assert_eq!(count, 45);

    let mut count = 0;
    let mut range = 0_usize..10;
    for i in range {
        assert!(i >= 0 && i < 10);
        count += i;
    }
    assert_eq!(count, 45);

    let mut count = 0;
    let mut rf = 3_usize..;
    for i in rf.take(10) {
        assert!(i >= 3 && i < 13);
        count += i;
    }
    assert_eq!(count, 75);

    let _ = 0_usize..4 + 4 - 3;

    fn foo() -> isize {
        42
    }
    let _ = 0..foo();

    let _ = { &42..&100 }; // references to literals are OK
    let _ = ..42_usize;

    // Test we can use two different types with a common supertype.
    let x = &42;
    {
        let y = 42;
        let _ = x..&y;
    }
}

#[test]
#[allow(dead_code)]
fn test_range_syntax_in_return_statement() {
    fn return_range_to() -> RangeTo<i32> {
        return ..1;
    }
    fn return_full_range() -> RangeFull {
        return ..;
    }
    // Not much to test.
}
