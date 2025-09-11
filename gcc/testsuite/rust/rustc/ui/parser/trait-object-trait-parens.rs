trait Trait<'a> {}

trait Obj {}

fn f<T: (Copy) + (?Sized) + (for<'a> Trait<'a>)>() {}

fn main() {
    let _: Box<(Obj) + (?Sized) + (for<'a> Trait<'a>)>;
// { dg-error ".E0225." "" { target *-*-* } .-1 }
// { dg-error ".E0225." "" { target *-*-* } .-2 }
// { dg-warning ".E0225." "" { target *-*-* } .-3 }
    let _: Box<(?Sized) + (for<'a> Trait<'a>) + (Obj)>;
// { dg-error ".E0225." "" { target *-*-* } .-1 }
// { dg-error ".E0225." "" { target *-*-* } .-2 }
// { dg-warning ".E0225." "" { target *-*-* } .-3 }
    let _: Box<(for<'a> Trait<'a>) + (Obj) + (?Sized)>;
// { dg-error ".E0225." "" { target *-*-* } .-1 }
// { dg-error ".E0225." "" { target *-*-* } .-2 }
// { dg-warning ".E0225." "" { target *-*-* } .-3 }
}

