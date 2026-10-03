union U {
    principal: u8,
}

impl U {
    fn calculate(&self) {}
}

fn main() {
    let u = U { principle: 0 };
// { dg-error ".E0560." "" { target *-*-* } .-1 }
// { help ".E0560." "" { target *-*-* } .-2 }
// { suggestion ".E0560." "" { target *-*-* } .-3 }
    let w = u.principial; // { dg-error ".E0609." "" { target *-*-* } }
// { help "" "" { target *-*-* } .-2 }
// { suggestion "" "" { target *-*-* } .-3 }

    let y = u.calculate; // { dg-error ".E0615." "" { target *-*-* } }
// { help "" "" { target *-*-* } .-2 }
// { suggestion "" "" { target *-*-* } .-3 }
}

