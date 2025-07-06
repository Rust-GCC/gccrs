macro_rules! a {
    () => { "a" }
}

macro_rules! b {
    ($doc:expr) => {
        #[doc = $doc]
         struct _B;
    }
}

b!(a!());

fn main() {}
