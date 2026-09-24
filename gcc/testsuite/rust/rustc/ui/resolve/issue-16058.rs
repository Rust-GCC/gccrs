// ignore-sgx std::os::fortanix_sgx::usercalls::raw::Result changes compiler suggestions

pub struct GslResult {
    pub val: f64,
    pub err: f64
}

impl GslResult {
    pub fn new() -> GslResult {
        Result {
// { dg-error ".E0574." "" { target *-*-* } .-1 }
            val: 0f64,
            err: 0f64
        }
    }
}

fn main() {}

