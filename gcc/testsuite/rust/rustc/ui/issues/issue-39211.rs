#![feature(associated_consts)]

trait VecN {
    const DIM: usize;
}
trait Mat {
    type Row: VecN;
}

fn m<M: Mat>() {
    let a = [3; M::Row::DIM];
// { dg-error "" "" { target *-*-* } .-1 }
}
fn main() {
}

