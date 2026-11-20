mod cat {
    pub struct Cat {
        meows: usize
    }

    pub fn new_cat() -> Cat {
        Cat { meows: 52 }
    }
}

fn main() {
    let nyan = cat::new_cat();
    assert_eq!(nyan.meows, 52);    // { dg-error ".E0616." "" { target *-*-* } }
}

