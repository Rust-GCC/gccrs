fn main() {
    // bad arguments to the format! call

    // bad number of arguments, see #44954 (originally #15780)

    format!("{}");
// { dg-error "" "" { target *-*-* } .-1 }

    format!("{1}", 1);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

    format!("{} {}");
// { dg-error "" "" { target *-*-* } .-1 }

    format!("{0} {1}", 1);
// { dg-error "" "" { target *-*-* } .-1 }

    format!("{0} {1} {2}", 1, 2);
// { dg-error "" "" { target *-*-* } .-1 }

    format!("{} {value} {} {}", 1, value=2);
// { dg-error "" "" { target *-*-* } .-1 }
    format!("{name} {value} {} {} {} {} {} {}", 0, name=1, value=2);
// { dg-error "" "" { target *-*-* } .-1 }

    format!("{} {foo} {} {bar} {}", 1, 2, 3);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

    format!("{foo}");                // { dg-error "" "" { target *-*-* } }
    format!("", 1, 2);               // { dg-error "" "" { target *-*-* } }
    format!("{}", 1, 2);             // { dg-error "" "" { target *-*-* } }
    format!("{1}", 1, 2);            // { dg-error "" "" { target *-*-* } }
    format!("{}", 1, foo=2);         // { dg-error "" "" { target *-*-* } }
    format!("{foo}", 1, foo=2);      // { dg-error "" "" { target *-*-* } }
    format!("", foo=2);              // { dg-error "" "" { target *-*-* } }
    format!("{} {}", 1, 2, foo=1, bar=2);  // { dg-error "" "" { target *-*-* } }

    format!("{foo}", foo=1, foo=2);  // { dg-error "" "" { target *-*-* } }
    format!("{foo} {} {}", foo=1, 2);   // { dg-error "" "" { target *-*-* } }

    // bad named arguments, #35082

    format!("{valuea} {valueb}", valuea=5, valuec=7);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

    // bad syntax of the format string

    format!("{"); // { dg-error "" "" { target *-*-* } }

    format!("foo } bar"); // { dg-error "" "" { target *-*-* } }
    format!("foo }"); // { dg-error "" "" { target *-*-* } }

    format!("foo %s baz", "bar"); // { dg-error "" "" { target *-*-* } }

    format!(r##"

        {foo}

    "##);
// { dg-error "" "" { target *-*-* } .-3 }

    // bad syntax in format string with multiple newlines, #53836
    format!("first number: {}
second number: {}
third number: {}
fourth number: {}
fifth number: {}
sixth number: {}
seventh number: {}
eighth number: {}
ninth number: {
tenth number: {}",
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
// { dg-error "" "" { target *-*-* } .-2 }
    println!("{} {:.*} {}", 1, 3.2, 4);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
    println!("{} {:07$.*} {}", 1, 3.2, 4);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
    println!("{} {:07$} {}", 1, 3.2, 4);
// { dg-error "" "" { target *-*-* } .-1 }
    println!("{:foo}", 1); // { dg-error "" "" { target *-*-* } }
    println!("{5} {:4$} {6:7$}", 1);
// { dg-error "" "" { target *-*-* } .-1 }

    // We used to ICE here because we tried to unconditionally access the first argument, which
    // doesn't exist.
    println!("{:.*}");
// { dg-error "" "" { target *-*-* } .-1 }
}

