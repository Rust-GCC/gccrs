// revisions: full min
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

struct PhantomU8<const X: u8>;

trait FxpStorage {
    type SInt; // Add arithmetic traits as needed.
}

macro_rules! fxp_storage_impls {
    ($($($n:literal)|+ => $sint:ty),* $(,)?) => {
        $($(impl FxpStorage for PhantomU8<$n> {
            type SInt = $sint;
        })*)*
    }
}

fxp_storage_impls! {
    1 => i8,
    2 => i16,
    3 | 4 => i32,
    5 | 6 | 7 | 8 => i64,
    9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 => i128,
}

type FxpStorageHelper<const INT_BITS: u8, const FRAC_BITS: u8> =
    PhantomU8<{(INT_BITS + FRAC_BITS + 7) / 8}>;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

struct Fxp<const INT_BITS: u8, const FRAC_BITS: u8>
where
    FxpStorageHelper<INT_BITS, FRAC_BITS>: FxpStorage,
// { dg-error "" "" { target *-*-* } .-1 }
{
    storage: <FxpStorageHelper<INT_BITS, FRAC_BITS> as FxpStorage>::SInt,
}

fn main() {
    Fxp::<1, 15> { storage: 0i16 };
    Fxp::<2, 15> { storage: 0i32 };
}

