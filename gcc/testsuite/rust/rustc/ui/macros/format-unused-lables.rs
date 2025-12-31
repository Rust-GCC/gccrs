fn main() {
    println!("Test", 123, 456, 789);
// { dg-error "" "" { target *-*-* } .-1 }

    println!("Test2",
        123,  // { dg-error "" "" { target *-*-* } }
        456,
        789
    );

    println!("Some stuff", UNUSED="args"); // { dg-error "" "" { target *-*-* } }

    println!("Some more $STUFF",
        "woo!",  // { dg-error "" "" { target *-*-* } }
            STUFF=
       "things"
             , UNUSED="args");
}

