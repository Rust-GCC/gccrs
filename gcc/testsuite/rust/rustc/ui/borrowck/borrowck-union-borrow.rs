// ignore-tidy-linelength

#[derive(Clone, Copy)]
union U {
    a: u8,
    b: u64,
}

fn main() {
    unsafe {
        let mut u = U { b: 0 };
        // Imm borrow, same field
        {
            let ra = &u.a;
            let ra2 = &u.a; // OK
            drop(ra);
        }
        {
            let ra = &u.a;
            let a = u.a; // OK
            drop(ra);
        }
        {
            let ra = &u.a;
            let rma = &mut u.a; // { dg-error ".E0502." "" { target *-*-* } }
            drop(ra);
        }
        {
            let ra = &u.a;
            u.a = 1; // { dg-error ".E0506." "" { target *-*-* } }
            drop(ra);
        }
        // Imm borrow, other field
        {
            let ra = &u.a;
            let rb = &u.b; // OK
            drop(ra);
        }
        {
            let ra = &u.a;
            let b = u.b; // OK
            drop(ra);
        }
        {
            let ra = &u.a;
            let rmb = &mut u.b; // { dg-error ".E0502." "" { target *-*-* } }
            drop(ra);
        }
        {
            let ra = &u.a;
            u.b = 1; // { dg-error ".E0506." "" { target *-*-* } }
            drop(ra);
        }
        // Mut borrow, same field
        {
            let rma = &mut u.a;
            let ra = &u.a; // { dg-error ".E0502." "" { target *-*-* } }
            drop(rma);
        }
        {
            let ra = &mut u.a;
            let a = u.a; // { dg-error ".E0503." "" { target *-*-* } }
            drop(ra);
        }
        {
            let rma = &mut u.a;
            let rma2 = &mut u.a; // { dg-error ".E0499." "" { target *-*-* } }
            drop(rma);
        }
        {
            let rma = &mut u.a;
            u.a = 1; // { dg-error ".E0506." "" { target *-*-* } }
            drop(rma);
        }
        // Mut borrow, other field
        {
            let rma = &mut u.a;
            let rb = &u.b; // { dg-error ".E0502." "" { target *-*-* } }
            drop(rma);
        }
        {
            let ra = &mut u.a;
            let b = u.b; // { dg-error ".E0503." "" { target *-*-* } }

            drop(ra);
        }
        {
            let rma = &mut u.a;
            let rmb2 = &mut u.b; // { dg-error ".E0499." "" { target *-*-* } }
            drop(rma);
        }
        {
            let rma = &mut u.a;
            u.b = 1; // { dg-error ".E0506." "" { target *-*-* } }
            drop(rma);
        }
    }
}

