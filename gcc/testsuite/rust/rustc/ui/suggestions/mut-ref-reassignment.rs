fn suggestion(opt: &mut Option<String>) {
    opt = None; // { dg-error ".E0308." "" { target *-*-* } }
}

fn no_suggestion(opt: &mut Result<String, ()>) {
    opt = None // { dg-error ".E0308." "" { target *-*-* } }
}

fn suggestion2(opt: &mut Option<String>) {
    opt = Some(String::new())// { dg-error ".E0308." "" { target *-*-* } }
}

fn no_suggestion2(opt: &mut Option<String>) {
    opt = Some(42)// { dg-error ".E0308." "" { target *-*-* } }
}

fn main() {}

