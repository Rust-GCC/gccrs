static a: &'static str = "foo";
static b: *const u8 = a as *const u8; // { dg-error ".E0606." "" { target *-*-* } }
static c: *const u8 = &a as *const u8; // { dg-error ".E0606." "" { target *-*-* } }

fn main() {
}

