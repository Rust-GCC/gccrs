// gate-test-const_raw_ptr_deref

const REG_ADDR: *const u8 = 0x5f3759df as *const u8;

const VALUE: u8 = unsafe { *REG_ADDR };
// { dg-error ".E0658." "" { target *-*-* } .-1 }

fn main() {
}

