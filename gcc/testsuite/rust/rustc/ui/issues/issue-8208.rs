use self::*; // { dg-error ".E0432." "" { target *-*-* } }
// { dg-error ".E0432." "" { target *-*-* } .-1 }

mod foo {
    use foo::*; // { dg-error ".E0432." "" { target *-*-* } }
// { dg-error ".E0432." "" { target *-*-* } .-1 }

    mod bar {
        use super::bar::*;
// { dg-error ".E0432." "" { target *-*-* } .-1 }
// { dg-error ".E0432." "" { target *-*-* } .-2 }
    }

}

fn main() {
}

