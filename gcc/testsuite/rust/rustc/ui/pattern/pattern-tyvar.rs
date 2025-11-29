enum Bar { T1((), Option<Vec<isize>>), T2 }

fn foo(t: Bar) {
    match t {
      Bar::T1(_, Some::<isize>(x)) => { // { dg-error ".E0308." "" { target *-*-* } }
        println!("{}", x);
      }
      _ => { panic!(); }
    }
}

fn main() { }

