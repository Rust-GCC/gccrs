fn gratitude() {
    let for_you = false;
    if not for_you {
// { dg-error "" "" { target *-*-* } .-1 }
        println!("I couldn't");
    }
}

fn qualification() {
    let the_worst = true;
    while not the_worst {
// { dg-error "" "" { target *-*-* } .-1 }
        println!("still pretty bad");
    }
}

fn should_we() {
    let not = true;
    if not  // lack of braces is [sic]
        println!("Then when?");
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

fn sleepy() {
    let resource = not 2;
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {
    let be_smothered_out_before = true;
    let young_souls = not be_smothered_out_before;
// { dg-error "" "" { target *-*-* } .-1 }
}

