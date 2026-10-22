// edition:2018

struct HasLifetime<'a>(&'a bool);

async fn error(lt: HasLifetime) { // { dg-error ".E0726." "" { target *-*-* } }
    if *lt.0 {}
}

fn no_error(lt: HasLifetime) {
    if *lt.0 {}
}

fn main() {}

