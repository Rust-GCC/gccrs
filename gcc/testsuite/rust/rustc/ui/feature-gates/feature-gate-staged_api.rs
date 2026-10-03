#![stable(feature = "a", since = "b")]
// { dg-error ".E0734." "" { target *-*-* } .-1 }
mod inner_private_module {
    // UnnameableTypeAlias isn't marked as reachable, so no stability annotation is required here
    pub type UnnameableTypeAlias = u8;
}

#[stable(feature = "a", since = "b")]
// { dg-error ".E0734." "" { target *-*-* } .-1 }
pub fn f() -> inner_private_module::UnnameableTypeAlias {
    0
}

fn main() {}

