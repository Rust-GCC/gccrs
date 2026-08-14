struct Session {
    opts: u8,
}

fn main() {
    let sess: &Session = &Session { opts: 0 };
    (sess as *const Session).opts; // { dg-error ".E0609." "" { target *-*-* } }

    let x = [0u32];
    (x as [u32; 1]).0; // { dg-error ".E0609." "" { target *-*-* } }
}

