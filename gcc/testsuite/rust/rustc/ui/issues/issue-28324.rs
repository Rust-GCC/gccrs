extern {
    static error_message_count: u32;
}

pub static BAZ: u32 = *&error_message_count;
// { dg-error ".E0133." "" { target *-*-* } .-1 }

fn main() {}

