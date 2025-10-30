// Make sure that we check that impl trait types implement the traits that they
// claim to.

#![feature(type_alias_impl_trait)]

type X<'a> = impl Into<&'static str> + From<&'a str>;
// { dg-error ".E0308." "" { target *-*-* } .-1 }

fn f<'a: 'static>(t: &'a str) -> X<'a> {
// { dg-warning "" "" { target *-*-* } .-1 }
    t
}

fn extend_lt<'a>(o: &'a str) -> &'static str {
    X::<'_>::from(o).into()
}

fn main() {
    let r =
    {
        let s = "abcdef".to_string();
        extend_lt(&s)
    };
    println!("{}", r);
}

