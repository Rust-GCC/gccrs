// revisions: full min
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

struct Test(*const usize);

type PassArg = ();

unsafe extern "C" fn pass(args: PassArg) {
    println!("Hello, world!");
}

impl Test {
    pub fn call_me<Args: Sized, const IDX: usize, const FN: unsafe extern "C" fn(Args)>(&self) {
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
        self.0 = Self::trampiline::<Args, IDX, FN> as _
    }

    unsafe extern "C" fn trampiline<
        Args: Sized,
        const IDX: usize,
        const FN: unsafe extern "C" fn(Args),
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    >(
        args: Args,
    ) {
        FN(args)
    }
}

fn main() {
    let x = Test();
    x.call_me::<PassArg, 30, pass>()
}

