// edition:2018
// compile-flags:-Cincremental=tmp/issue-72442

use std::fs::File;
use std::future::Future;
use std::io::prelude::*;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    block_on(async {
        {
            let path = std::path::Path::new(".");
            let mut f = File::open(path.to_str())?;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
            let mut src = String::new();
            f.read_to_string(&mut src)?;
            Ok(())
        }
    })
}

fn block_on<F>(f: F) -> F::Output
where
    F: Future<Output = Result<(), Box<dyn std::error::Error>>>,
{
    Ok(())
}

