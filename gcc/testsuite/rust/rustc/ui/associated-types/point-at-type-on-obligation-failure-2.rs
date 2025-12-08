trait Bar {}

trait Foo {
    type Assoc: Bar;
}

impl Foo for () {
    type Assoc = bool; // { dg-error ".E0277." "" { target *-*-* } }
}

trait Baz
where
    Self::Assoc: Bar,
{
    type Assoc;
}

impl Baz for () {
    type Assoc = bool; // { dg-error ".E0277." "" { target *-*-* } }
}

trait Bat
where
    <Self as Bat>::Assoc: Bar,
{
    type Assoc;
}

impl Bat for () {
    type Assoc = bool; // { dg-error ".E0277." "" { target *-*-* } }
}

fn main() {}

