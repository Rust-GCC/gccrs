fn get_closure() -> Box<dyn Fn() -> Vec<u8>> {
    let vec = vec![1u8, 2u8];

    let closure = move || { // { dg-error ".E0525." "" { target *-*-* } }
        vec
    };

    Box::new(closure)
}

fn main() {}

