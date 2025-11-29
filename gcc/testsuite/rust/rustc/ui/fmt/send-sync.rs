fn send<T: Send>(_: T) {}
fn sync<T: Sync>(_: T) {}

fn main() {
    // `Cell` is not `Sync`, so `&Cell` is neither `Sync` nor `Send`,
    // `std::fmt::Arguments` used to forget this...
    let c = std::cell::Cell::new(42);
    send(format_args!("{:?}", c)); // { dg-error ".E0277." "" { target *-*-* } }
    sync(format_args!("{:?}", c)); // { dg-error ".E0277." "" { target *-*-* } }
}

