// edition:2018

use std::io::Error;

fn make_unit() -> Result<(), Error> {
    Ok(())
}

fn main() {
    let fut = async {
        make_unit()?; // { dg-error ".E0282." "" { target *-*-* } }

        Ok(())
    };
}

