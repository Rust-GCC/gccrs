fn a<'a>() -> &'a isize {
    let vec = vec![1, 2, 3, 4];
    let vec: &[isize] = &vec;
    let tail = match vec {
        &[_a, ref tail @ ..] => &tail[0],
        _ => panic!("foo")
    };
    tail // { dg-error ".E0515." "" { target *-*-* } }
}

fn main() {
    let fifth = a();
    println!("{}", *fifth);
}

