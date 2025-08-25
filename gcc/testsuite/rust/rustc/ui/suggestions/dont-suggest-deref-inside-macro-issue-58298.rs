fn warn(_: &str) {}

macro_rules! intrinsic_match {
    ($intrinsic:expr) => {
        warn(format!("unsupported intrinsic {}", $intrinsic));
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    };
}

fn main() {
    intrinsic_match! {
        "abc"
    };
}

