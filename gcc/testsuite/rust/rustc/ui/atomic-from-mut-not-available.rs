// only-x86
// only-linux

fn main() {
    core::sync::atomic::AtomicU64::from_mut(&mut 0u64);
// { dg-error ".E0599." "" { target *-*-* } .-1 }
}

