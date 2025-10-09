static c3: char =
    '\x1' // { dg-error "" "" { target *-*-* } }
;

static s3: &'static str =
    "\x1" // { dg-error "" "" { target *-*-* } }
;

static c: char =
    '\●' // { dg-error "" "" { target *-*-* } }
;

static s: &'static str =
    "\●" // { dg-error "" "" { target *-*-* } }
;

fn main() {}

