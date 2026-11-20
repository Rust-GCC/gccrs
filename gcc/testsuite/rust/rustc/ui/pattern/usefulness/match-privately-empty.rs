#![feature(never_type)]
#![feature(exhaustive_patterns)]

mod private {
    pub struct Private {
        _bot: !,
        pub misc: bool,
    }
    pub const DATA: Option<Private> = None;
}

fn main() {
    match private::DATA {
// { dg-error ".E0004." "" { target *-*-* } .-1 }
        None => {}
        Some(private::Private {
            misc: false,
            ..
        }) => {}
    }
}

