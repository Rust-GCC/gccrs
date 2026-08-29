// edition:2018

use std::convert::{TryFrom, TryInto};
use std::io;

pub struct MyStream;
pub struct OtherStream;

pub async fn connect() -> io::Result<MyStream> {
    let stream: MyStream = OtherStream.try_into()?;
    Ok(stream)
}

impl TryFrom<OtherStream> for MyStream {}
// { dg-error ".E0046." "" { target *-*-* } .-1 }

fn main() {}

