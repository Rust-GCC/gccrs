#![allow(illegal_floating_point_literal_pattern)]

enum T { A, B }

fn main() {
    let x = T::A;
    match x { T::B => { } } // { dg-error ".E0004." "" { target *-*-* } }
    match true { // { dg-error ".E0004." "" { target *-*-* } }
      true => {}
    }
    match Some(10) { // { dg-error ".E0004." "" { target *-*-* } }
      None => {}
    }
    match (2, 3, 4) { // { dg-error ".E0004." "" { target *-*-* } }
                      //  and `(_, _, 5_i32..=i32::MAX)` not covered
      (_, _, 4) => {}
    }
    match (T::A, T::A) { // { dg-error ".E0004." "" { target *-*-* } }
      (T::A, T::B) => {}
      (T::B, T::A) => {}
    }
    match T::A { // { dg-error ".E0004." "" { target *-*-* } }
      T::A => {}
    }
    // This is exhaustive, though the algorithm got it wrong at one point
    match (T::A, T::B) {
      (T::A, _) => {}
      (_, T::A) => {}
      (T::B, T::B) => {}
    }
    let vec = vec![Some(42), None, Some(21)];
    let vec: &[Option<isize>] = &vec;
    match *vec { // { dg-error ".E0004." "" { target *-*-* } }
        [Some(..), None, ref tail @ ..] => {}
        [Some(..), Some(..), ref tail @ ..] => {}
        [None] => {}
    }
    let vec = vec![1];
    let vec: &[isize] = &vec;
    match *vec {
        [_, ref tail @ ..] => (),
        [] => ()
    }
    let vec = vec![0.5f32];
    let vec: &[f32] = &vec;
    match *vec { // { dg-error ".E0004." "" { target *-*-* } }
        [0.1, 0.2, 0.3] => (),
        [0.1, 0.2] => (),
        [0.1] => (),
        [] => ()
    }
    let vec = vec![Some(42), None, Some(21)];
    let vec: &[Option<isize>] = &vec;
    match *vec {
        [Some(..), None, ref tail @ ..] => {}
        [Some(..), Some(..), ref tail @ ..] => {}
        [None, None, ref tail @ ..] => {}
        [None, Some(..), ref tail @ ..] => {}
        [Some(_)] => {}
        [None] => {}
        [] => {}
    }
}

