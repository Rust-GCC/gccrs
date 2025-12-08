trait Foo {}

impl Foo for dyn Send {}

impl Foo for dyn Send + Send {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

impl Foo for dyn Send + Sync {}

impl Foo for dyn Sync + Send {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

impl Foo for dyn Send + Sync + Send {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

fn main() {}

