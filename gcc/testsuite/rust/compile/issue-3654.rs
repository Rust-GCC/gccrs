#[lang = "sized"]
pub trait Sized {}

#[lang = "fn_once"]
pub trait FnOnce<Args> {
    #[lang = "fn_once_output"]
    type Output;

    extern "rust-call" fn call_once(self, args: Args) -> Self::Output;
}

type Meeshka = Mow<!>;
// { dg-error "generic arguments are not allowed for this type .E0109." "" { target *-*-* } .-1 }
type Mow = &'static fn(!) -> !;
