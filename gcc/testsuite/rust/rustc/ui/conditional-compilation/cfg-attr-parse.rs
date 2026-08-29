// Parse `cfg_attr` with varying numbers of attributes and trailing commas

// Completely empty `cfg_attr` input
#[cfg_attr()] // { dg-error "" "" { target *-*-* } }
struct NoConfigurationPredicate;

// Zero attributes, zero trailing comma (comma manatory here)
#[cfg_attr(all())] // { dg-error "" "" { target *-*-* } }
struct A0C0;

// Zero attributes, one trailing comma
#[cfg_attr(all(),)] // Ok
struct A0C1;

// Zero attributes, two trailing commas
#[cfg_attr(all(),,)] // { dg-error "" "" { target *-*-* } }
struct A0C2;

// One attribute, no trailing comma
#[cfg_attr(all(), must_use)] // Ok
struct A1C0;

// One attribute, one trailing comma
#[cfg_attr(all(), must_use,)] // Ok
struct A1C1;

// One attribute, two trailing commas
#[cfg_attr(all(), must_use,,)] // { dg-error "" "" { target *-*-* } }
struct A1C2;

// Two attributes, no trailing comma
#[cfg_attr(all(), must_use, deprecated)] // Ok
struct A2C0;

// Two attributes, one trailing comma
#[cfg_attr(all(), must_use, deprecated,)] // Ok
struct A2C1;

// Two attributes, two trailing commas
#[cfg_attr(all(), must_use, deprecated,,)] // { dg-error "" "" { target *-*-* } }
struct A2C2;

// Wrong delimiter `[`
#[cfg_attr[all(),,]]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
struct BracketZero;

// Wrong delimiter `{`
#[cfg_attr{all(),,}]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
struct BraceZero;

fn main() {}

