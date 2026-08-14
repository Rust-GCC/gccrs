struct ErrorKind;
struct Error(ErrorKind);

impl From<nope::Thing> for Error { // { dg-error ".E0433." "" { target *-*-* } }
    fn from(_: nope::Thing) -> Self { // { dg-error ".E0433." "" { target *-*-* } }
        unimplemented!()
    }
}

impl From<ErrorKind> for Error {
    fn from(_: ErrorKind) -> Self {
        unimplemented!()
    }
}

fn main() {}

