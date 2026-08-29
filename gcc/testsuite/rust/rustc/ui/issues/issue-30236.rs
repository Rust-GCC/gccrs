type Foo<
    Unused // { dg-error ".E0091." "" { target *-*-* } }
    > = u8;

fn main() {

}

