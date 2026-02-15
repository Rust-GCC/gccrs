macro_rules! bad {
    ($s:ident whatever) => {
        {
            let $s = 0;
            *&mut $s = 0;
// { dg-error ".E0596." "" { target *-*-* } .-1 }
        }
    }
}

fn main() {
    bad!(foo whatever);
}

