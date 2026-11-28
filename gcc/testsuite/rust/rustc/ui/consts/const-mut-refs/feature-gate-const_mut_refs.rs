fn main() {
    foo(&mut 5);
}

const fn foo(x: &mut i32) -> i32 { // { dg-error ".E0658." "" { target *-*-* } }
    *x + 1

}

