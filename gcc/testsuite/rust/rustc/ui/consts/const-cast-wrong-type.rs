static a: [u8; 3] = ['h' as u8, 'i' as u8, 0 as u8];
static b: *const i8 = &a as *const i8; // { dg-error ".E0308." "" { target *-*-* } }

fn main() {
}

