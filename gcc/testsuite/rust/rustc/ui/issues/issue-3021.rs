trait SipHash {
    fn reset(&self);
}

fn siphash(k0 : u64) {
    struct SipState {
        v0: u64,
    }

    impl SipHash for SipState {
        fn reset(&self) {
           self.v0 = k0 ^ 0x736f6d6570736575; // { dg-error ".E0434." "" { target *-*-* } }
        }
    }
    panic!();
}

fn main() {}

