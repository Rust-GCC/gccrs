mod SomeModule {
    const PRIVATE: u32 = 0x_a_bad_1dea_u32;
}

fn main() {
    SomeModule::PRIVATE; // { dg-error ".E0603." "" { target *-*-* } }
}

