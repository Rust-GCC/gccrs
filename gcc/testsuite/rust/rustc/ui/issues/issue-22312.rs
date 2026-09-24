use std::ops::Index;

pub trait Array2D: Index<usize> + Sized {
    fn rows(&self) -> usize;
    fn columns(&self) -> usize;
    fn get<'a>(&'a self, y: usize, x: usize) -> Option<&'a <Self as Index<usize>>::Output> {
        if y >= self.rows() || x >= self.columns() {
            return None;
        }
        let i = y * self.columns() + x;
        let indexer = &(*self as &dyn Index<usize, Output = <Self as Index<usize>>::Output>);
// { dg-error ".E0605." "" { target *-*-* } .-1 }
        Some(indexer.index(i))
    }
}

fn main() {}

