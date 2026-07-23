// aux-build:use-from-trait-xc.rs

extern crate use_from_trait_xc;

use use_from_trait_xc::Trait::foo;
// { dg-error ".E0253." "" { target *-*-* } .-1 }

use use_from_trait_xc::Trait::Assoc;
// { dg-error ".E0253." "" { target *-*-* } .-1 }

use use_from_trait_xc::Trait::CONST;
// { dg-error ".E0253." "" { target *-*-* } .-1 }

use use_from_trait_xc::Foo::new; // { dg-error ".E0603." "" { target *-*-* } }
// { dg-error ".E0603." "" { target *-*-* } .-1 }

use use_from_trait_xc::Foo::C; // { dg-error ".E0603." "" { target *-*-* } }
// { dg-error ".E0603." "" { target *-*-* } .-1 }

use use_from_trait_xc::Bar::new as bnew;
// { dg-error ".E0432." "" { target *-*-* } .-1 }

use use_from_trait_xc::Baz::new as baznew;
// { dg-error ".E0432." "" { target *-*-* } .-1 }

fn main() {}

