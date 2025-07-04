#[lang = "sized"]
pub trait Sized {}

#[lang = "fn_once"]
pub trait FnOnce<Args> {
    #[lang = "fn_once_output"]
    type Output;

    extern "rust-call" fn call_once(self, args: Args) -> Self::Output;
}

pub trait Trait {
    pub fn nrvo(init: fn()) -> [u8; 4096] {
        let mut buf = [0; 4096];

        buf
    }
}
