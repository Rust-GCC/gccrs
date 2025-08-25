#![allow(dead_code)]

#![forbid(non_camel_case_types)]
#![feature(non_ascii_idents)]

// Some scripts (e.g., hiragana) don't have a concept of
// upper/lowercase

// 1. non_camel_case_types

// Can start with non-lowercase letter
struct Θχ;
struct ヒa;

struct χa;
// { dg-error "" "" { target *-*-* } .-1 }

// If there's already leading or trailing underscores, they get trimmed before checking.
// This is fine:
struct _ヒb;

// This is not:
struct __χa;
// { dg-error "" "" { target *-*-* } .-1 }

// Besides this, we cannot have two continuous underscores in the middle.

struct 对__否;
// { dg-error "" "" { target *-*-* } .-1 }

struct ヒ__χ;
// { dg-error "" "" { target *-*-* } .-1 }

// also cannot have lowercase letter next to a underscore.
// so this triggers the lint:

struct Hello_你好;
// { dg-error "" "" { target *-*-* } .-1 }

struct Hello_World;
// { dg-error "" "" { target *-*-* } .-1 }

struct 你_ӟ;
// { dg-error "" "" { target *-*-* } .-1 }

// and this is ok:

struct 你_好;

fn main() {}

