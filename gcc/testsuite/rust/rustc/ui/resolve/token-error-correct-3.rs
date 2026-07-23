// ignore-cloudabi no std::fs support

// Test that we do some basic error correction in the tokeniser (and don't spew
// too many bogus errors).

pub mod raw {
    use std::{io, fs};
    use std::path::Path;

    pub fn ensure_dir_exists<P: AsRef<Path>, F: FnOnce(&Path)>(path: P,
                                                               callback: F)
                                                               -> io::Result<bool> {
        if !is_directory(path.as_ref()) {
// { dg-error ".E0425." "" { target *-*-* } .-1 }
            callback(path.as_ref();
// { dg-error "" "" { target *-*-* } .-1 }
            fs::create_dir_all(path.as_ref()).map(|()| true)
        } else {
// { dg-error "" "" { target *-*-* } .-1 }
            Ok(false);
        }

        panic!();
    }
}

fn main() {}

