struct S(u8, (u8, u8));

fn main() {
    let s = S(0, (0, 0));

    s.1e1; // { dg-error ".E0609." "" { target *-*-* } }
    s.1.; // { dg-error "" "" { target *-*-* } }
    s.1.1;
    s.1.1e1; // { dg-error ".E0609." "" { target *-*-* } }
    { s.1e+; } // { dg-error "" "" { target *-*-* } }
// { dg-error ".E0609." "" { target *-*-* } .-2 }
// { dg-error ".E0609." "" { target *-*-* } .-3 }
    { s.1e-; } // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
    { s.1e+1; } // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
    { s.1e-1; } // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
    { s.1.1e+1; } // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
    { s.1.1e-1; } // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
    s.0x1e1;  // { dg-error ".E0609." "" { target *-*-* } }
    s.0x1.; // { dg-error ".E0609." "" { target *-*-* } }
// { dg-error ".E0609." "" { target *-*-* } .-2 }
// { dg-error ".E0609." "" { target *-*-* } .-3 }
    s.0x1.1; // { dg-error ".E0609." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
    s.0x1.1e1; // { dg-error ".E0609." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
    { s.0x1e+; } // { dg-error "" "" { target *-*-* } }
    { s.0x1e-; } // { dg-error "" "" { target *-*-* } }
    s.0x1e+1; // { dg-error ".E0609." "" { target *-*-* } }
    s.0x1e-1; // { dg-error ".E0609." "" { target *-*-* } }
    { s.0x1.1e+1; } // { dg-error "" "" { target *-*-* } }
// { dg-error ".E0609." "" { target *-*-* } .-2 }
// { dg-error ".E0609." "" { target *-*-* } .-3 }
    { s.0x1.1e-1; } // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
    s.1e1f32; // { dg-error ".E0609." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
    s.1.f32; // { dg-error ".E0609." "" { target *-*-* } }
    s.1.1f32; // { dg-error "" "" { target *-*-* } }
    s.1.1e1f32; // { dg-error ".E0609." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
    { s.1e+f32; } // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
    { s.1e-f32; } // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
    { s.1e+1f32; } // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
    { s.1e-1f32; } // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
    { s.1.1e+1f32; } // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
    { s.1.1e-1f32; } // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
}

