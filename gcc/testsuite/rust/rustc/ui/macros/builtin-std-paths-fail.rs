#[derive(
    core::RustcDecodable, // { dg-error ".E0433." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
    core::RustcDecodable, // { dg-error ".E0433." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
)]
#[core::bench] // { dg-error ".E0433." "" { target *-*-* } }
#[core::global_allocator] // { dg-error ".E0433." "" { target *-*-* } }
#[core::test_case] // { dg-error ".E0433." "" { target *-*-* } }
#[core::test] // { dg-error ".E0433." "" { target *-*-* } }
struct Core;

#[derive(
    std::RustcDecodable, // { dg-error ".E0433." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
    std::RustcDecodable, // { dg-error ".E0433." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
)]
#[std::bench] // { dg-error ".E0433." "" { target *-*-* } }
#[std::global_allocator] // { dg-error ".E0433." "" { target *-*-* } }
#[std::test_case] // { dg-error ".E0433." "" { target *-*-* } }
#[std::test] // { dg-error ".E0433." "" { target *-*-* } }
struct Std;

fn main() {}

