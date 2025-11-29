// edition:2018
// compile-flags: --crate-type lib

async fn no_move_across_await_struct() -> Vec<usize> {
    let s = Small { x: vec![31], y: vec![19, 1441] };
    needs_vec(s.x).await;
    s.x
// { dg-error ".E0382." "" { target *-*-* } .-1 }
}

struct Small {
    x: Vec<usize>,
    y: Vec<usize>,
}

async fn needs_vec(_vec: Vec<usize>) {}

