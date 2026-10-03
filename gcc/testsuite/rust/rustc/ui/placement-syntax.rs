fn main() {
    let x = -5;
    if x<-1 { // { dg-error "" "" { target *-*-* } }
        println!("ok");
    }
}

