// normalize-stderr-test: "not-a-file.md:.*\(" -> "not-a-file.md: $$FILE_NOT_FOUND_MSG ("

#![feature(external_doc)]

#[doc(include = "not-a-file.md")]
// { dg-error "" "" { target *-*-* } .-1 }

#[doc(include = "auxiliary/invalid-utf8.txt")]
// { dg-error "" "" { target *-*-* } .-1 }

#[doc(include)]
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
// { suggestion "" "" { target *-*-* } .-3 }

#[doc(include("../README.md"))]
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
// { suggestion "" "" { target *-*-* } .-3 }

#[doc(include = 123)]
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
// { suggestion "" "" { target *-*-* } .-3 }

#[doc(include(123))]
// { dg-error "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }
// { suggestion "" "" { target *-*-* } .-3 }

fn main() {}

