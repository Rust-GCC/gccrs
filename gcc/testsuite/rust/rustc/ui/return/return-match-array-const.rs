fn main() {
    [(); return match 0 { n => n }];
// { dg-error ".E0572." "" { target *-*-* } .-1 }

    [(); return match 0 { 0 => 0 }];
// { dg-error ".E0572." "" { target *-*-* } .-1 }

    [(); return match () { 'a' => 0, _ => 0 }];
// { dg-error ".E0572." "" { target *-*-* } .-1 }
}

