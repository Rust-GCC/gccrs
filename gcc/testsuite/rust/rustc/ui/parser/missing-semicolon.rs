macro_rules! m {
    ($($e1:expr),*; $($e2:expr),*) => {
        $( let x = $e1 )*; // { dg-error "" "" { target *-*-* } }
        $( println!("{}", $e2) )*;
    }
}

fn main() { m!(0, 0; 0, 0); }

