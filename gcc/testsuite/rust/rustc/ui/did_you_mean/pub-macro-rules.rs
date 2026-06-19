#[macro_use] mod bleh {
    pub macro_rules! foo { // { dg-error "" "" { target *-*-* } }
        ($n:ident) => (
            fn $n () -> i32 {
                1
            }
        )
    }

}

foo!(meh);

fn main() {
    println!("{}", meh());
}

