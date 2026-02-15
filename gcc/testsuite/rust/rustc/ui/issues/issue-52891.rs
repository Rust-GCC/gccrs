// aux-build:issue-52891.rs
// run-rustfix

#![allow(warnings)]

extern crate issue_52891;

// Check that we don't suggest renaming duplicate imports but instead
// suggest removing one.

use issue_52891::a;
use issue_52891::a; // { dg-error ".E0252." "" { target *-*-* } }

use issue_52891::{a, b, c}; // { dg-error ".E0252." "" { target *-*-* } }
use issue_52891::{d, a, e}; // { dg-error ".E0252." "" { target *-*-* } }
use issue_52891::{f, g, a}; // { dg-error ".E0252." "" { target *-*-* } }

use issue_52891::{a, // { dg-error ".E0252." "" { target *-*-* } }
    h,
    i};
use issue_52891::{j,
    a, // { dg-error ".E0252." "" { target *-*-* } }
    k};
use issue_52891::{l,
    m,
    a}; // { dg-error ".E0252." "" { target *-*-* } }

use issue_52891::a::inner;
use issue_52891::b::inner; // { dg-error ".E0252." "" { target *-*-* } }

use issue_52891::{self};
// { dg-error ".E0254." "" { target *-*-* } .-1 }

use issue_52891::n;
#[macro_use]
use issue_52891::n; // { dg-error ".E0252." "" { target *-*-* } }

fn main() {}

