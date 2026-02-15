// edition:2018
use core::future::Future;

async fn base_thing() -> Result<(), ()> {
    Ok(())
}

fn thing_one() -> impl Future<Output = Result<(), ()>> {
    base_thing()
}

fn thing_two() -> impl Future<Output = Result<(), ()>> {
    base_thing()
}

async fn thing() -> Result<(), ()> {
    if true {
        thing_one()
    } else {
        thing_two() // { dg-error ".E0308." "" { target *-*-* } }
    }.await
}

fn main() {}

