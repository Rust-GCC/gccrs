use self::Direction::{North, East, South, West};

#[derive(PartialEq, Eq)]
struct NewBool(bool);

#[derive(PartialEq, Eq)]
enum Direction {
    North,
    East,
    South,
    West
}

const TRUE_TRUE: (bool, bool) = (true, true);

fn nonexhaustive_1() {
    match (true, false) {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
        TRUE_TRUE => (),
        (false, false) => (),
        (false, true) => ()
    }
}

const NONE: Option<Direction> = None;
const EAST: Direction = East;

fn nonexhaustive_2() {
    match Some(Some(North)) {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
        Some(NONE) => (),
        Some(Some(North)) => (),
        Some(Some(EAST)) => (),
        Some(Some(South)) => (),
        None => ()
    }
}

const NEW_FALSE: NewBool = NewBool(false);
struct Foo {
    bar: Option<Direction>,
    baz: NewBool
}

const STATIC_FOO: Foo = Foo { bar: None, baz: NEW_FALSE };

fn nonexhaustive_3() {
    match (Foo { bar: Some(North), baz: NewBool(true) }) {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
        Foo { bar: None, baz: NewBool(true) } => (),
        Foo { bar: _, baz: NEW_FALSE } => (),
        Foo { bar: Some(West), baz: NewBool(true) } => (),
        Foo { bar: Some(South), .. } => (),
        Foo { bar: Some(EAST), .. } => ()
    }
}

fn main() {
    nonexhaustive_1();
    nonexhaustive_2();
    nonexhaustive_3();
}

