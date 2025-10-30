// run-pass

/// Tests access to the Unicode version constant.
pub fn main() {
    check(std::char::UNICODE_VERSION);
}

pub fn check(unicode_version: (u8, u8, u8)) {
    assert!(unicode_version.0 >= 10);
}

