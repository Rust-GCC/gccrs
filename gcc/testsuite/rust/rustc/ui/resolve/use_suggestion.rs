fn main() {
    let x1 = HashMap::new(); // { dg-error ".E0433." "" { target *-*-* } }
    let x2 = GooMap::new(); // { dg-error ".E0433." "" { target *-*-* } }

    let y1: HashMap; // { dg-error ".E0412." "" { target *-*-* } }
    let y2: GooMap; // { dg-error ".E0412." "" { target *-*-* } }
}

