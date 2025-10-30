// gate-test-const_impl_trait

struct AlanTuring<T>(T);
const fn no_rpit2() -> AlanTuring<impl std::fmt::Debug> { // { dg-error ".E0658." "" { target *-*-* } }
    AlanTuring(0)
}

const fn no_rpit() -> impl std::fmt::Debug {} // { dg-error ".E0658." "" { target *-*-* } }

fn main() {}

