trait A: Sized {
    type N;
    fn x() ->
        Self<
          N= // { dg-error ".E0229." "" { target *-*-* } }
          Self::N> {
        loop {}
    }
    fn y(&self) ->
        std
           <N=()> // { dg-error ".E0229." "" { target *-*-* } }
           ::option::Option<()>
    { None }
    fn z(&self) ->
        u32<N=()> // { dg-error ".E0229." "" { target *-*-* } }
    { 42 }

}

fn main() {
}

