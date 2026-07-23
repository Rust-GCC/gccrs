struct CantCopyThis;

struct IWantToCopyThis {
    but_i_cant: CantCopyThis,
}

impl Copy for IWantToCopyThis {}
// { dg-error ".E0204." "" { target *-*-* } .-1 }

enum CantCopyThisEither {
    A,
    B,
}

enum IWantToCopyThisToo {
    ButICant(CantCopyThisEither),
}

impl Copy for IWantToCopyThisToo {}
// { dg-error ".E0204." "" { target *-*-* } .-1 }

fn main() {}

