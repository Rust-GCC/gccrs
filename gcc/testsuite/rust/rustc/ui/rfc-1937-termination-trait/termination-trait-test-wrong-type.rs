// compile-flags: --test

use std::num::ParseFloatError;

#[test]
fn can_parse_zero_as_f32() -> Result<f32, ParseFloatError> { // { dg-error ".E0277." "" { target *-*-* } }
    "0".parse()
}

