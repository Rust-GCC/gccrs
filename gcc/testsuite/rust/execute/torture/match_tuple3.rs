enum Foo {
  A,
  B,
}

fn inspect(f: ((Foo, i32), (Foo, i32))) -> i32
{
  match f {
    ((Foo::A, 1), (Foo::B, _)) => { return 1; }
    ((Foo::A, _), (Foo::A, 10)) => { return 10; }
    ((Foo::A, _), (Foo::A, _)) => { return 100; }
    ((Foo::B, 5), _) => { return 1000; }
    _ => { return 100000; }
  }
}

fn main() -> i32 {
  inspect(((Foo::A, 1), (Foo::B, 3))) + inspect(((Foo::B, 5), (Foo::A, 10))) - 1001
}