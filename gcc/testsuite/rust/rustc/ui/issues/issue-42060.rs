fn main() {
    let thing = ();
    let other: typeof(thing) = thing; // { dg-error ".E0516." "" { target *-*-* } }
// { dg-error ".E0516." "" { target *-*-* } .-1 }
}

fn f(){
    let q = 1;
    <typeof(q)>::N // { dg-error ".E0516." "" { target *-*-* } }
// { dg-error ".E0516." "" { target *-*-* } .-1 }
}

