mod a {
  pub enum Enum {
    EnumStructVariant { x: u8, y: u8, z: u8 }
  }

  pub fn get_enum_struct_variant() -> () {
    Enum::EnumStructVariant { x: 1, y: 2, z: 3 }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
  }
}

mod b {
  mod test {
    use a;

    fn test_enum_struct_variant() {
      let enum_struct_variant = ::a::get_enum_struct_variant();
      match enum_struct_variant {
        a::Enum::EnumStructVariant { x, y, z } => {
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
        }
      }
    }
  }
}

fn main() {}

