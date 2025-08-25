// check-pass

#![feature(associated_type_bounds)]

type _TaWhere1<T> where T: Iterator<Item: Copy> = T; // { dg-warning "" "" { target *-*-* } }
type _TaWhere2<T> where T: Iterator<Item: 'static> = T; // { dg-warning "" "" { target *-*-* } }
type _TaWhere3<T> where T: Iterator<Item: 'static> = T; // { dg-warning "" "" { target *-*-* } }
type _TaWhere4<T> where T: Iterator<Item: 'static + Copy + Send> = T; // { dg-warning "" "" { target *-*-* } }
type _TaWhere5<T> where T: Iterator<Item: for<'a> Into<&'a u8>> = T; // { dg-warning "" "" { target *-*-* } }
type _TaWhere6<T> where T: Iterator<Item: Iterator<Item: Copy>> = T; // { dg-warning "" "" { target *-*-* } }

type _TaInline1<T: Iterator<Item: Copy>> = T; // { dg-warning "" "" { target *-*-* } }
type _TaInline2<T: Iterator<Item: 'static>> = T; // { dg-warning "" "" { target *-*-* } }
type _TaInline3<T: Iterator<Item: 'static>> = T; // { dg-warning "" "" { target *-*-* } }
type _TaInline4<T: Iterator<Item: 'static + Copy + Send>> = T; // { dg-warning "" "" { target *-*-* } }
type _TaInline5<T: Iterator<Item: for<'a> Into<&'a u8>>> = T; // { dg-warning "" "" { target *-*-* } }
type _TaInline6<T: Iterator<Item: Iterator<Item: Copy>>> = T; // { dg-warning "" "" { target *-*-* } }

fn main() {}

