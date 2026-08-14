// ignore-tidy-tab

fn main() {
    format!("{
    a");
// { dg-error "" "" { target *-*-* } .-1 }
    format!("{ \

    b");
// { dg-error "" "" { target *-*-* } .-1 }
    format!(r#"{ \

    rawc"#);
// { dg-error "" "" { target *-*-* } .-3 }
    format!(r#"{ \n
\n
    rawd"#);
// { dg-error "" "" { target *-*-* } .-3 }
    format!("{ \n
\n
    e");
// { dg-error "" "" { target *-*-* } .-1 }
    format!("
    {
    a");
// { dg-error "" "" { target *-*-* } .-1 }
    format!("
    {
    a
    ");
// { dg-error "" "" { target *-*-* } .-2 }
    format!("  \
    { \
    	\
    b");
// { dg-error "" "" { target *-*-* } .-1 }
    format!("  \
    { \
    	\
    b \

    ");
// { dg-error "" "" { target *-*-* } .-3 }
    format!(r#"
raw  { \

    c"#);
// { dg-error "" "" { target *-*-* } .-3 }
    format!(r#"
raw  { \n
\n
    d"#);
// { dg-error "" "" { target *-*-* } .-3 }
    format!("
  { \n
\n
    e");
// { dg-error "" "" { target *-*-* } .-1 }

    format!("
    {asdf
    }
    ", asdf=1);
    // ok - this is supported
    format!("
    {
    asdf}
    ", asdf=1);
// { dg-error "" "" { target *-*-* } .-2 }
    println!("\t{}");
// { dg-error "" "" { target *-*-* } .-1 }

    // note: `\x7B` is `{`
    println!("\x7B}\u{8} {", 1);
// { dg-error "" "" { target *-*-* } .-1 }

    println!("\x7B}\u8 {", 1);
// { dg-error "" "" { target *-*-* } .-1 }

    // note: raw strings don't escape `\xFF` and `\u{FF}` sequences
    println!(r#"\x7B}\u{8} {"#, 1);
// { dg-error "" "" { target *-*-* } .-1 }

    println!(r#"\x7B}\u8 {"#, 1);
// { dg-error "" "" { target *-*-* } .-1 }
}

