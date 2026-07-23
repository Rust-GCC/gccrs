trait NewTrait : SomeNonExistentTrait {}
// { dg-error ".E0405." "" { target *-*-* } .-1 }

impl SomeNonExistentTrait for isize {}
// { dg-error ".E0405." "" { target *-*-* } .-1 }

fn f<T:SomeNonExistentTrait>() {}
// { dg-error ".E0405." "" { target *-*-* } .-1 }

fn main() {}

