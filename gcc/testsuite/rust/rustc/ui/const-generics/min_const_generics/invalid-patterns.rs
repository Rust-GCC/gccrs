#![feature(min_const_generics)]
use std::mem::transmute;

fn get_flag<const FlagSet: bool, const ShortName: char>() -> Option<char> {
  if FlagSet {
    Some(ShortName)
  } else {
    None
  }
}

union CharRaw {
  byte: u8,
  character: char,
}

union BoolRaw {
  byte: u8,
  boolean: bool,
}

const char_raw: CharRaw = CharRaw { byte: 0xFF };
const bool_raw: BoolRaw = BoolRaw { byte: 0x42 };

fn main() {
  // Test that basic cases don't work
  assert!(get_flag::<true, 'c'>().is_some());
  assert!(get_flag::<false, 'x'>().is_none());
  get_flag::<false, 0xFF>();
// { dg-error ".E0308." "" { target *-*-* } .-1 }
  get_flag::<7, 'c'>();
// { dg-error ".E0308." "" { target *-*-* } .-1 }
  get_flag::<42, 0x5ad>();
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }


  get_flag::<false, { unsafe { char_raw.character } }>();
// { dg-error ".E0080." "" { target *-*-* } .-1 }
  get_flag::<{ unsafe { bool_raw.boolean } }, 'z'>();
// { dg-error ".E0080." "" { target *-*-* } .-1 }
  get_flag::<{ unsafe { bool_raw.boolean } }, { unsafe { char_raw.character } }>();
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// { dg-error ".E0080." "" { target *-*-* } .-2 }
}

