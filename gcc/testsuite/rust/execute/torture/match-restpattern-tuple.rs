fn main() -> i32 {
    let x = (1, 2, 3, 4);
    let mut ret = 1;

    match x {
        (1, .., 2, 4) => { /* should not take this path */ },
        (2, ..) => { /* should not take this path */ },
        (b, .., 4) => { ret -= b; },
        _ => {}
    }

    ret
}