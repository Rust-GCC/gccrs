trait SomeTrait {
    fn foo(); // { dg-error "" "" { target *-*-* } }
}

fn main() {
    let trait_obj: &dyn SomeTrait = SomeTrait;
// { dg-error ".E0038." "" { target *-*-* } .-1 }
// { dg-error ".E0038." "" { target *-*-* } .-2 }

    let &invalid = trait_obj;
// { dg-error ".E0033." "" { target *-*-* } .-1 }
}

