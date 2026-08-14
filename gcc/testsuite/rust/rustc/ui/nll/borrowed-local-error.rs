fn gimme(x: &(u32,)) -> &u32 {
    &x.0
}

fn main() {
    let x = gimme({
        let v = (22,);
        &v
// { dg-error ".E0597." "" { target *-*-* } .-1 }
    });
    println!("{:?}", x);
}

