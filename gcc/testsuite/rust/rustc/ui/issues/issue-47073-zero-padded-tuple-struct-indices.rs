type Guilty = bool;
type FineDollars = u32;

struct Verdict(Guilty, Option<FineDollars>);

fn main() {
    let justice = Verdict(true, Some(2718));
    let _condemned = justice.00;
// { dg-error ".E0609." "" { target *-*-* } .-1 }
    let _punishment = justice.001;
// { dg-error ".E0609." "" { target *-*-* } .-1 }
}

