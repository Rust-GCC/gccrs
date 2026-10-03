impl ! {} // OK
impl ! where u8: Copy {} // OK

impl Trait Type {} // { dg-error "" "" { target *-*-* } }
impl Trait .. {} // { dg-error "" "" { target *-*-* } }
impl ?Sized for Type {} // { dg-error "" "" { target *-*-* } }
impl ?Sized for .. {} // { dg-error "" "" { target *-*-* } }

default unsafe FAIL // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }

