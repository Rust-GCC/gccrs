mod strategy {
    mod dragon;
    mod grisu;
}

use core::num::flt2dec::MAX_SIG_DIGITS;
use core::num::flt2dec::{decode, DecodableFloat, Decoded, FullDecoded};
use std::io::Write;
use std::vec::Vec;
use test::Bencher;

pub fn decode_finite<T: DecodableFloat>(v: T) -> Decoded {
    match decode(v).1 {
        FullDecoded::Finite(decoded) => decoded,
        full_decoded => panic!("expected finite, got {:?} instead", full_decoded),
    }
}

#[bench]
fn bench_small_shortest(b: &mut Bencher) {
    let mut buf = Vec::with_capacity(20);

    b.iter(|| {
        buf.clear();
        write!(&mut buf, "{}", 3.1415926f64).unwrap()
    });
}

#[bench]
fn bench_big_shortest(b: &mut Bencher) {
    let mut buf = Vec::with_capacity(300);

    b.iter(|| {
        buf.clear();
        write!(&mut buf, "{}", f64::MAX).unwrap()
    });
}
