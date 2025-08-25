trait T<'x> {
    type V;
}

impl<'g> T<'g> for u32 {
    type V = u16;
}

fn main() {
    (&|_| ()) as &dyn for<'x> Fn(<u32 as T<'x>>::V);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
}

