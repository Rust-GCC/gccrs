enum F { G(isize, isize) }

enum H { I(J, K) }

enum J { L(isize, isize) }
enum K { M(isize, isize) }

fn main()
{

    let _z = match F::G(1, 2) {
      F::G(x, x) => { println!("{}", x + x); }
// { dg-error ".E0416." "" { target *-*-* } .-1 }
    };

    let _z = match H::I(J::L(1, 2), K::M(3, 4)) {
      H::I(J::L(x, _), K::M(_, x))
// { dg-error ".E0416." "" { target *-*-* } .-1 }
        => { println!("{}", x + x); }
    };

    let _z = match (1, 2) {
        (x, x) => { x } // { dg-error ".E0416." "" { target *-*-* } }
    };

}

