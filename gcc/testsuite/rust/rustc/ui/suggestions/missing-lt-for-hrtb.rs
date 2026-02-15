struct X<'a>(&'a ());
struct S<'a>(&'a dyn Fn(&X) -> &X);
// { dg-error ".E0106." "" { target *-*-* } .-1 }
// { dg-error ".E0106." "" { target *-*-* } .-2 }
struct V<'a>(&'a dyn for<'b> Fn(&X) -> &X);
// { dg-error ".E0106." "" { target *-*-* } .-1 }
// { dg-error ".E0106." "" { target *-*-* } .-2 }

fn main() {
    let x = S(&|x| {
        println!("hi");
        x
    });
    x.0(&X(&()));
}

