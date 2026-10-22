enum Either<T, U> { Left(T), Right(U) }

struct X(Either<(usize,usize), fn()>);

impl X {
    pub fn with<F>(&self, blk: F) where F: FnOnce(&Either<(usize, usize), fn()>) {
        let X(ref e) = *self;
        blk(e)
    }
}

fn main() {
    let mut x = X(Either::Right(main as fn()));
    (&mut x).with(
        |opt| { // { dg-error ".E0499." "" { target *-*-* } }
            match opt {
                &Either::Right(ref f) => {
                    x = X(Either::Left((0, 0)));
                    (*f)()
                },
                _ => panic!()
            }
        })
}

