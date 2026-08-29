fn is_send<T: Send>(val: T) {}

fn use_impl_sync(val: impl Sync) {
    is_send(val); // { dg-error ".E0277." "" { target *-*-* } }
}

fn use_where<S>(val: S) where S: Sync {
    is_send(val); // { dg-error ".E0277." "" { target *-*-* } }
}

fn use_bound<S: Sync>(val: S) {
    is_send(val); // { dg-error ".E0277." "" { target *-*-* } }
}

fn use_bound_2<
    S // Make sure we can synthezise a correct suggestion span for this case
    :
    Sync
>(val: S) {
    is_send(val); // { dg-error ".E0277." "" { target *-*-* } }
}

fn use_bound_and_where<S: Sync>(val: S) where S: std::fmt::Debug {
    is_send(val); // { dg-error ".E0277." "" { target *-*-* } }
}

fn use_unbound<S>(val: S) {
    is_send(val); // { dg-error ".E0277." "" { target *-*-* } }
}

fn main() {}

