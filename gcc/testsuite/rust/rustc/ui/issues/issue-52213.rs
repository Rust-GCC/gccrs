fn transmute_lifetime<'a, 'b, T>(t: &'a (T,)) -> &'b T {
    match (&t,) { // { dg-error ".E0495." "" { target *-*-* } }
        ((u,),) => u,
    }
}

fn main() {
    let x = {
        let y = Box::new((42,));
        transmute_lifetime(&y)
    };

    println!("{}", x);
}

