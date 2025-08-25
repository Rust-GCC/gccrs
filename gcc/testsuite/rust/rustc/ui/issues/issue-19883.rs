trait From<Src> {
    type Output;

    fn from(src: Src) -> <Self as From<Src>>::Output;
}

trait To: Sized {
    fn to<Dst: From<Self>>(self) ->
        <Dst as From<Self>>::Dst
// { dg-error ".E0576." "" { target *-*-* } .-1 }
    {
        From::from(self)
    }
}

fn main() {}

