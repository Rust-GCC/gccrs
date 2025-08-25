type A where 'a: 'b + 'c = u8; // OK
type A where 'a: 'b, = u8; // OK
type A where 'a: = u8; // OK
type A where 'a:, = u8; // OK
type A where 'a: 'b + 'c = u8; // OK
type A where = u8; // OK
type A where 'a: 'b + = u8; // OK
type A where , = u8; // { dg-error "" "" { target *-*-* } }

fn main() {}

