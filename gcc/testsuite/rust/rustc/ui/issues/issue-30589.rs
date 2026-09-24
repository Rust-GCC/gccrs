use std::fmt;

impl fmt::Display for DecoderError { // { dg-error ".E0412." "" { target *-*-* } }
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "Missing data: {}", self.0)
    }
}
fn main() {
}

