trait MatrixShape {}

struct Col<D, C> {
    data: D,
    col: C,
}

trait Collection { fn len(&self) -> usize; }

impl<T, M: MatrixShape> Collection for Col<M, usize> {
// { dg-error ".E0207." "" { target *-*-* } .-1 }
    fn len(&self) -> usize {
        unimplemented!()
    }
}

fn main() {}

