use std::cell::RefCell;

fn main() {
    let mut y = 1;
    let c = RefCell::new(vec![]);
    c.push(Box::new(|| y = 0));
    c.push(Box::new(|| y = 0));
// { dg-error ".E0499." "" { target *-*-* } .-1 }
}

fn ufcs() {
    let mut y = 1;
    let c = RefCell::new(vec![]);

    Push::push(&c, Box::new(|| y = 0));
    Push::push(&c, Box::new(|| y = 0));
// { dg-error ".E0499." "" { target *-*-* } .-1 }
}

trait Push<'c> {
    fn push<'f: 'c>(&self, push: Box<dyn FnMut() + 'f>);
}

impl<'c> Push<'c> for RefCell<Vec<Box<dyn FnMut() + 'c>>> {
    fn push<'f: 'c>(&self, fun: Box<dyn FnMut() + 'f>) {
        self.borrow_mut().push(fun)
    }
}

