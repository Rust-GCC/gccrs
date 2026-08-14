use bar::Foo; // { dg-error ".E0432." "" { target *-*-* } }
// { dg-error ".E0432." "" { target *-*-* } .-1 }
mod bar {
    use Foo;
}

fn main() {}

