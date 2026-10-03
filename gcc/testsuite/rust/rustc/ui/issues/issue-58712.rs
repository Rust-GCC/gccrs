struct AddrVec<H, A> {
    h: H,
    a: A,
}

impl<H> AddrVec<H, DeviceId> {
// { dg-error ".E0412." "" { target *-*-* } .-1 }
    pub fn device(&self) -> DeviceId {
// { dg-error ".E0412." "" { target *-*-* } .-1 }
        self.tail()
    }
}

fn main() {}

