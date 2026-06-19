#![feature(half_open_range_patterns)]

fn main() {}

#[cfg(FALSE)] fn e() { let _ = box #![attr] 0; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { let _ = [#[attr]]; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { let _ = foo#[attr](); }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { let _ = foo(#![attr]); }
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
#[cfg(FALSE)] fn e() { let _ = x.foo(#![attr]); }
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
#[cfg(FALSE)] fn e() { let _ = 0 + #![attr] 0; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { let _ = !#![attr] 0; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { let _ = -#![attr] 0; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { let _ = x #![attr] as Y; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { let _ = || #![attr] foo; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { let _ = move || #![attr] foo; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { let _ = || #![attr] {foo}; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { let _ = move || #![attr] {foo}; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { let _ = #[attr] ..#[attr] 0; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { let _ = #[attr] ..; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { let _ = #[attr] &#![attr] 0; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { let _ = #[attr] &mut #![attr] 0; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { let _ = if 0 #[attr] {}; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { let _ = if 0 {#![attr]}; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { let _ = if 0 {} #[attr] else {}; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { let _ = if 0 {} else #[attr] {}; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { let _ = if 0 {} else {#![attr]}; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { let _ = if 0 {} else #[attr] if 0 {}; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { let _ = if 0 {} else if 0 #[attr] {}; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { let _ = if 0 {} else if 0 {#![attr]}; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { let _ = if let _ = 0 #[attr] {}; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { let _ = if let _ = 0 {#![attr]}; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { let _ = if let _ = 0 {} #[attr] else {}; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { let _ = if let _ = 0 {} else #[attr] {}; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { let _ = if let _ = 0 {} else {#![attr]}; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { let _ = if let _ = 0 {} else #[attr] if let _ = 0 {}; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { let _ = if let _ = 0 {} else if let _ = 0 #[attr] {}; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { let _ = if let _ = 0 {} else if let _ = 0 {#![attr]}; }
// { dg-error "" "" { target *-*-* } .-1 }

#[cfg(FALSE)] fn s() { #[attr] #![attr] let _ = 0; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn s() { #[attr] #![attr] 0; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn s() { #[attr] #![attr] foo!(); }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn s() { #[attr] #![attr] foo![]; }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn s() { #[attr] #![attr] foo!{}; }
// { dg-error "" "" { target *-*-* } .-1 }

// FIXME: Allow attributes in pattern constexprs?
// note: requires parens in patterns to allow disambiguation

#[cfg(FALSE)] fn e() { match 0 { 0..=#[attr] 10 => () } }
// { dg-error ".E0586." "" { target *-*-* } .-1 }
// { dg-error ".E0586." "" { target *-*-* } .-2 }
#[cfg(FALSE)] fn e() { match 0 { 0..=#[attr] -10 => () } }
// { dg-error ".E0586." "" { target *-*-* } .-1 }
// { dg-error ".E0586." "" { target *-*-* } .-2 }
#[cfg(FALSE)] fn e() { match 0 { 0..=-#[attr] 10 => () } }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { match 0 { 0..=#[attr] FOO => () } }
// { dg-error ".E0586." "" { target *-*-* } .-1 }
// { dg-error ".E0586." "" { target *-*-* } .-2 }

#[cfg(FALSE)] fn e() { let _ = x.#![attr]foo(); }
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
#[cfg(FALSE)] fn e() { let _ = x.#[attr]foo(); }
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

// make sure we don't catch this bug again...
#[cfg(FALSE)] fn e() { { fn foo() { #[attr]; } } }
// { dg-error "" "" { target *-*-* } .-1 }
#[cfg(FALSE)] fn e() { { fn foo() { #[attr] } } }
// { dg-error "" "" { target *-*-* } .-1 }

