// check-pass

#![feature(box_syntax)]
#![feature(box_patterns)]
#![warn(unused)] // UI tests pass `-A unused` (#43896)

struct SoulHistory {
    corridors_of_light: usize,
    hours_are_suns: bool,
    endless_and_singing: bool
}

struct LovelyAmbition {
    lips: usize,
    fire: usize
}

#[derive(Clone, Copy)]
enum Large {
    Suit { case: () }
}

struct Tuple(Large, ());

fn main() {
    let i_think_continually = 2; // { dg-warning "" "" { target *-*-* } }
    let who_from_the_womb_remembered = SoulHistory {
        corridors_of_light: 5,
        hours_are_suns: true,
        endless_and_singing: true
    };

    let mut mut_unused_var = 1;
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    let (mut var, unused_var) = (1, 2);
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }
    // NOTE: `var` comes after `unused_var` lexicographically yet the warning
    // for `var` will be emitted before the one for `unused_var`. We use an
    // `IndexMap` to ensure this is the case instead of a `BTreeMap`.

    if let SoulHistory { corridors_of_light, // { dg-warning "" "" { target *-*-* } }
                         mut hours_are_suns, // { dg-warning "" "" { target *-*-* } }
                         endless_and_singing: true } = who_from_the_womb_remembered {
        hours_are_suns = false; // { dg-warning "" "" { target *-*-* } }
    }

    let the_spirit = LovelyAmbition { lips: 1, fire: 2 };
    let LovelyAmbition { lips, fire } = the_spirit; // { dg-warning "" "" { target *-*-* } }
    println!("{}", lips);

    let bag = Large::Suit {
        case: ()
    };

    // Plain struct
    match bag {
        Large::Suit { case } => {} // { dg-warning "" "" { target *-*-* } }
    };

    // Referenced struct
    match &bag {
        &Large::Suit { case } => {} // { dg-warning "" "" { target *-*-* } }
    };

    // Boxed struct
    match box bag {
        box Large::Suit { case } => {} // { dg-warning "" "" { target *-*-* } }
    };

    // Tuple with struct
    match (bag,) {
        (Large::Suit { case },) => {} // { dg-warning "" "" { target *-*-* } }
    };

    // Slice with struct
    match [bag] {
        [Large::Suit { case }] => {} // { dg-warning "" "" { target *-*-* } }
    };

    // Tuple struct with struct
    match Tuple(bag, ()) {
        Tuple(Large::Suit { case }, ()) => {} // { dg-warning "" "" { target *-*-* } }
    };
}

