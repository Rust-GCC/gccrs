fn main() {
    let mut v = vec![1];
    let mut f = || v.push(2);
    let _w = v; // { dg-error ".E0505." "" { target *-*-* } }

    f();
}

