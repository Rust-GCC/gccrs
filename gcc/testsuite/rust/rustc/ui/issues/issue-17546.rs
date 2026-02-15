// ignore-sgx std::os::fortanix_sgx::usercalls::raw::Result changes compiler suggestions

use foo::MyEnum::Result;
use foo::NoResult; // Through a re-export

mod foo {
    pub use self::MyEnum::NoResult;

    pub enum MyEnum {
        Result,
        NoResult
    }

    fn new() -> NoResult<MyEnum, String> {
// { dg-error ".E0573." "" { target *-*-* } .-1 }
        unimplemented!()
    }
}

mod bar {
    use foo::MyEnum::Result;
    use foo;

    fn new() -> Result<foo::MyEnum, String> {
// { dg-error ".E0573." "" { target *-*-* } .-1 }
        unimplemented!()
    }
}

fn new() -> Result<foo::MyEnum, String> {
// { dg-error ".E0573." "" { target *-*-* } .-1 }
    unimplemented!()
}

fn newer() -> NoResult<foo::MyEnum, String> {
// { dg-error ".E0573." "" { target *-*-* } .-1 }
    unimplemented!()
}

fn main() {
    let _ = new();
}

