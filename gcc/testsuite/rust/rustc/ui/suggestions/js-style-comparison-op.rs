// run-rustfix
fn main() {
    if 1 === 1 { // { dg-error "" "" { target *-*-* } }
        println!("yup!");
    } else if 1 !== 1 { // { dg-error "" "" { target *-*-* } }
        println!("nope!");
    }
}

