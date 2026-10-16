fn main () {
    let map = NonExistingMap::new(); // { dg-error ".E0433." "" { target *-*-* } }
}

