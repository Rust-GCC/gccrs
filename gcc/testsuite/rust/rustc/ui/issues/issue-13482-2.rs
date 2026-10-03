// compile-flags:-Z verbose

fn main() {
    let x = [1,2];
    let y = match x {
        [] => None, // { dg-error ".E0527." "" { target *-*-* } }
        [a,_] => Some(a)
    };
}

