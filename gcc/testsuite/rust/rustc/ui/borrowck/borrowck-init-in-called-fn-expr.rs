fn main() {
    let j = || -> isize {
        let i: isize;
        i // { dg-error ".E0381." "" { target *-*-* } }
    };
    j();
}

