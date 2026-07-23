fn main() {
    println!("%.*3$s %s!\n", "Hello,", "World", 4); // { dg-error "" "" { target *-*-* } }
    println!("%1$*2$.*3$f", 123.456); // { dg-error "" "" { target *-*-* } }
    println!(r###"%.*3$s
        %s!\n
"###, "Hello,", "World", 4);
// { dg-error "" "" { target *-*-* } .-1 }
    // correctly account for raw strings in inline suggestions

    // This should *not* produce hints, on the basis that there's equally as
    // many "correct" format specifiers.  It's *probably* just an actual typo.
    println!("{} %f", "one", 2.0); // { dg-error "" "" { target *-*-* } }

    println!("Hi there, $NAME.", NAME="Tim"); // { dg-error "" "" { target *-*-* } }
    println!("$1 $0 $$ $NAME", 1, 2, NAME=3);
// { dg-error "" "" { target *-*-* } .-1 }
}

