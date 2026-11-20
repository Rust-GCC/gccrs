fn a<'a>() -> &'a [isize] {
    let vec = vec![1, 2, 3, 4];
    let vec: &[isize] = &vec;
    let tail = match vec {
        &[_, ref tail @ ..] => tail,
        _ => panic!("a")
    };
    tail // { dg-error ".E0515." "" { target *-*-* } }
}

fn b<'a>() -> &'a [isize] {
    let vec = vec![1, 2, 3, 4];
    let vec: &[isize] = &vec;
    let init = match vec {
        &[ref init @ .., _] => init,
        _ => panic!("b")
    };
    init // { dg-error ".E0515." "" { target *-*-* } }
}

fn c<'a>() -> &'a [isize] {
    let vec = vec![1, 2, 3, 4];
    let vec: &[isize] = &vec;
    let slice = match vec {
        &[_, ref slice @ .., _] => slice,
        _ => panic!("c")
    };
    slice // { dg-error ".E0515." "" { target *-*-* } }
}

fn main() {}

