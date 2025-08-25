// produce special borrowck message inside all kinds of loops

struct FuncWrapper<'a, T : 'a> {
    func : fn(&'a mut T) -> ()
}

impl<'a, T : 'a> FuncWrapper<'a, T> {
    fn in_loop(self, arg : &'a mut T) {
        loop {
            (self.func)(arg) // { dg-error ".E0499." "" { target *-*-* } }
        }
    }

    fn in_while(self, arg : &'a mut T) {
        while true { // { dg-warning "" "" { target *-*-* } }
            (self.func)(arg) // { dg-error ".E0499." "" { target *-*-* } }
        }
    }

    fn in_for(self, arg : &'a mut T) {
        let v : Vec<()> = vec![];
        for _ in v.iter() {
            (self.func)(arg) // { dg-error ".E0499." "" { target *-*-* } }
        }
    }
}

fn main() {
}

