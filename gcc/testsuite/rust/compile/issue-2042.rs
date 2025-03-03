#[lang = "sized"]
pub trait Sized {}

#[lang = "fn_once"]
pub trait FnOnce<Args> {
    #[lang = "fn_once_output"]
    type Output;

    extern "rust-call" fn call_once(self, args: Args) -> Self::Output;
}


fn f<'r>(p: &'r mut fn(p: &mut ())) {
    (*p)(())
    // { dg-error "expected .&mut ()." "" { target *-*-* } .-1 }
}

fn main() {}
