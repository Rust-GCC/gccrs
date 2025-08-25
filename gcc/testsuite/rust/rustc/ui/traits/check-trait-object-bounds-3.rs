// Check that we validate associated type bounds for trait objects

trait X<'a> {
    type Y: Into<&'static str> + From<&'a str>;
}

fn f<'a, T: X<'a> + ?Sized>(s: &'a str) -> &'static str {
    T::Y::from(s).into()
}

pub fn main() {
    let z;
    {
        let s = String::from("abcdef");
        z = f::<dyn X<Y = &str>>(&s);
// { dg-error ".E0597." "" { target *-*-* } .-1 }
    }

    println!("{}", z)
}

