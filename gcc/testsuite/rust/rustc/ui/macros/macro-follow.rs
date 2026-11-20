//
// Check the macro follow sets (see corresponding rpass test).

#![allow(unused_macros)]

// FOLLOW(pat) = {FatArrow, Comma, Eq, Or, Ident(if), Ident(in)}
macro_rules! follow_pat {
    ($p:pat ()) => {};       // { dg-error "" "" { target *-*-* } }
    ($p:pat []) => {};       // { dg-error "" "" { target *-*-* } }
    ($p:pat {}) => {};       // { dg-error "" "" { target *-*-* } }
    ($p:pat :) => {};        // { dg-error "" "" { target *-*-* } }
    ($p:pat >) => {};        // { dg-error "" "" { target *-*-* } }
    ($p:pat +) => {};        // { dg-error "" "" { target *-*-* } }
    ($p:pat ident) => {};    // { dg-error "" "" { target *-*-* } }
    ($p:pat $q:pat) => {};   // { dg-error "" "" { target *-*-* } }
    ($p:pat $e:expr) => {};  // { dg-error "" "" { target *-*-* } }
    ($p:pat $t:ty) => {};    // { dg-error "" "" { target *-*-* } }
    ($p:pat $s:stmt) => {};  // { dg-error "" "" { target *-*-* } }
    ($p:pat $q:path) => {};  // { dg-error "" "" { target *-*-* } }
    ($p:pat $b:block) => {}; // { dg-error "" "" { target *-*-* } }
    ($p:pat $i:ident) => {}; // { dg-error "" "" { target *-*-* } }
    ($p:pat $t:tt) => {};    // { dg-error "" "" { target *-*-* } }
    ($p:pat $i:item) => {};  // { dg-error "" "" { target *-*-* } }
    ($p:pat $m:meta) => {};  // { dg-error "" "" { target *-*-* } }
}
// FOLLOW(expr) = {FatArrow, Comma, Semicolon}
macro_rules! follow_expr {
    ($e:expr ()) => {};       // { dg-error "" "" { target *-*-* } }
    ($e:expr []) => {};       // { dg-error "" "" { target *-*-* } }
    ($e:expr {}) => {};       // { dg-error "" "" { target *-*-* } }
    ($e:expr =) => {};        // { dg-error "" "" { target *-*-* } }
    ($e:expr |) => {};        // { dg-error "" "" { target *-*-* } }
    ($e:expr :) => {};        // { dg-error "" "" { target *-*-* } }
    ($e:expr >) => {};        // { dg-error "" "" { target *-*-* } }
    ($e:expr +) => {};        // { dg-error "" "" { target *-*-* } }
    ($e:expr ident) => {};    // { dg-error "" "" { target *-*-* } }
    ($e:expr if) => {};       // { dg-error "" "" { target *-*-* } }
    ($e:expr in) => {};       // { dg-error "" "" { target *-*-* } }
    ($e:expr $p:pat) => {};   // { dg-error "" "" { target *-*-* } }
    ($e:expr $f:expr) => {};  // { dg-error "" "" { target *-*-* } }
    ($e:expr $t:ty) => {};    // { dg-error "" "" { target *-*-* } }
    ($e:expr $s:stmt) => {};  // { dg-error "" "" { target *-*-* } }
    ($e:expr $p:path) => {};  // { dg-error "" "" { target *-*-* } }
    ($e:expr $b:block) => {}; // { dg-error "" "" { target *-*-* } }
    ($e:expr $i:ident) => {}; // { dg-error "" "" { target *-*-* } }
    ($e:expr $t:tt) => {};    // { dg-error "" "" { target *-*-* } }
    ($e:expr $i:item) => {};  // { dg-error "" "" { target *-*-* } }
    ($e:expr $m:meta) => {};  // { dg-error "" "" { target *-*-* } }
}
// FOLLOW(ty) = {OpenDelim(Brace), Comma, FatArrow, Colon, Eq, Gt, Semi, Or,
//               Ident(as), Ident(where), OpenDelim(Bracket), Nonterminal(Block)}
macro_rules! follow_ty {
    ($t:ty ()) => {};       // { dg-error "" "" { target *-*-* } }
    ($t:ty []) => {};       // ok (RFC 1462)
    ($t:ty +) => {};        // { dg-error "" "" { target *-*-* } }
    ($t:ty ident) => {};    // { dg-error "" "" { target *-*-* } }
    ($t:ty if) => {};       // { dg-error "" "" { target *-*-* } }
    ($t:ty $p:pat) => {};   // { dg-error "" "" { target *-*-* } }
    ($t:ty $e:expr) => {};  // { dg-error "" "" { target *-*-* } }
    ($t:ty $r:ty) => {};    // { dg-error "" "" { target *-*-* } }
    ($t:ty $s:stmt) => {};  // { dg-error "" "" { target *-*-* } }
    ($t:ty $p:path) => {};  // { dg-error "" "" { target *-*-* } }
    ($t:ty $b:block) => {}; // ok (RFC 1494)
    ($t:ty $i:ident) => {}; // { dg-error "" "" { target *-*-* } }
    ($t:ty $r:tt) => {};    // { dg-error "" "" { target *-*-* } }
    ($t:ty $i:item) => {};  // { dg-error "" "" { target *-*-* } }
    ($t:ty $m:meta) => {};  // { dg-error "" "" { target *-*-* } }
}
// FOLLOW(stmt) = FOLLOW(expr)
macro_rules! follow_stmt {
    ($s:stmt ()) => {};       // { dg-error "" "" { target *-*-* } }
    ($s:stmt []) => {};       // { dg-error "" "" { target *-*-* } }
    ($s:stmt {}) => {};       // { dg-error "" "" { target *-*-* } }
    ($s:stmt =) => {};        // { dg-error "" "" { target *-*-* } }
    ($s:stmt |) => {};        // { dg-error "" "" { target *-*-* } }
    ($s:stmt :) => {};        // { dg-error "" "" { target *-*-* } }
    ($s:stmt >) => {};        // { dg-error "" "" { target *-*-* } }
    ($s:stmt +) => {};        // { dg-error "" "" { target *-*-* } }
    ($s:stmt ident) => {};    // { dg-error "" "" { target *-*-* } }
    ($s:stmt if) => {};       // { dg-error "" "" { target *-*-* } }
    ($s:stmt in) => {};       // { dg-error "" "" { target *-*-* } }
    ($s:stmt $p:pat) => {};   // { dg-error "" "" { target *-*-* } }
    ($s:stmt $e:expr) => {};  // { dg-error "" "" { target *-*-* } }
    ($s:stmt $t:ty) => {};    // { dg-error "" "" { target *-*-* } }
    ($s:stmt $t:stmt) => {};  // { dg-error "" "" { target *-*-* } }
    ($s:stmt $p:path) => {};  // { dg-error "" "" { target *-*-* } }
    ($s:stmt $b:block) => {}; // { dg-error "" "" { target *-*-* } }
    ($s:stmt $i:ident) => {}; // { dg-error "" "" { target *-*-* } }
    ($s:stmt $t:tt) => {};    // { dg-error "" "" { target *-*-* } }
    ($s:stmt $i:item) => {};  // { dg-error "" "" { target *-*-* } }
    ($s:stmt $m:meta) => {};  // { dg-error "" "" { target *-*-* } }
}
// FOLLOW(path) = FOLLOW(ty)
macro_rules! follow_path {
    ($p:path ()) => {};       // { dg-error "" "" { target *-*-* } }
    ($p:path []) => {};       // ok (RFC 1462)
    ($p:path +) => {};        // { dg-error "" "" { target *-*-* } }
    ($p:path ident) => {};    // { dg-error "" "" { target *-*-* } }
    ($p:path if) => {};       // { dg-error "" "" { target *-*-* } }
    ($p:path $q:pat) => {};   // { dg-error "" "" { target *-*-* } }
    ($p:path $e:expr) => {};  // { dg-error "" "" { target *-*-* } }
    ($p:path $t:ty) => {};    // { dg-error "" "" { target *-*-* } }
    ($p:path $s:stmt) => {};  // { dg-error "" "" { target *-*-* } }
    ($p:path $q:path) => {};  // { dg-error "" "" { target *-*-* } }
    ($p:path $b:block) => {}; // ok (RFC 1494)
    ($p:path $i:ident) => {}; // { dg-error "" "" { target *-*-* } }
    ($p:path $t:tt) => {};    // { dg-error "" "" { target *-*-* } }
    ($p:path $i:item) => {};  // { dg-error "" "" { target *-*-* } }
    ($p:path $m:meta) => {};  // { dg-error "" "" { target *-*-* } }
}
// FOLLOW(block) = any token
// FOLLOW(ident) = any token

fn main() {}

