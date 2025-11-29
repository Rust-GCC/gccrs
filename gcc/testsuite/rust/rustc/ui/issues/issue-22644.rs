#![feature(type_ascription)]

fn main() {
    let a : usize = 0;
    let long_name : usize = 0;

    println!("{}", a as usize > long_name);
    println!("{}", a as usize < long_name); // { dg-error "" "" { target *-*-* } }
    println!("{}{}", a as usize < long_name, long_name);
// { dg-error "" "" { target *-*-* } .-1 }
    println!("{}", a as usize < 4); // { dg-error "" "" { target *-*-* } }
    println!("{}", a: usize > long_name);
    println!("{}{}", a: usize < long_name, long_name);
// { dg-error "" "" { target *-*-* } .-1 }
    println!("{}", a: usize < 4); // { dg-error "" "" { target *-*-* } }

    println!("{}", a
                   as
                   usize
                   < // { dg-error "" "" { target *-*-* } }
                   4);
    println!("{}", a


                   as


                   usize
                   < // { dg-error "" "" { target *-*-* } }
                   5);

    println!("{}", a as usize << long_name); // { dg-error "" "" { target *-*-* } }

    println!("{}", a: &mut 4); // { dg-error "" "" { target *-*-* } }
}

