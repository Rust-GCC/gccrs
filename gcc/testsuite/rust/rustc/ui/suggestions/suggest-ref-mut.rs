struct X(usize);

impl X {
    fn zap(&self) {
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
        self.0 = 32;
// { dg-error ".E0594." "" { target *-*-* } .-1 }
    }
}

fn main() {
    let ref foo = 16;
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
    *foo = 32;
// { dg-error ".E0594." "" { target *-*-* } .-1 }
    if let Some(ref bar) = Some(16) {
// { help "" "" { target *-*-* } .-1 }
// { suggestion "" "" { target *-*-* } .-2 }
        *bar = 32;
// { dg-error ".E0594." "" { target *-*-* } .-1 }
    }
    match 16 {
        ref quo => { *quo = 32; },
// { dg-error ".E0594." "" { target *-*-* } .-1 }
// { help ".E0594." "" { target *-*-* } .-2 }
// { suggestion ".E0594." "" { target *-*-* } .-3 }
    }
}

