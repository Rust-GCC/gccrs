fn main() {}

macro_rules! expand_to_enum {
    () => {
        enum BadE {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
    };
}

macro_rules! mac_impl {
    ($($i:item)*) => {
        struct S;
        impl S { $($i)* }
    }
}

mac_impl! {
    struct BadS; // { dg-error "" "" { target *-*-* } }
    expand_to_enum!();
}

macro_rules! mac_trait {
    ($($i:item)*) => {
        trait T { $($i)* }
    }
}

mac_trait! {
    struct BadS; // { dg-error "" "" { target *-*-* } }
    expand_to_enum!();
}

macro_rules! mac_extern {
    ($($i:item)*) => {
        extern "C" { $($i)* }
    }
}

mac_extern! {
    struct BadS; // { dg-error "" "" { target *-*-* } }
    expand_to_enum!();
}

