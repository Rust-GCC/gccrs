fn id<T>(x: T) -> T { x }

struct StateMachineIter<'a> {
    statefn: &'a StateMachineFunc<'a>
}

type StateMachineFunc<'a> = fn(&mut StateMachineIter<'a>) -> Option<&'static str>;

impl<'a> Iterator for StateMachineIter<'a> {
    type Item = &'static str;

    fn next(&mut self) -> Option<&'static str> {
        return  (*self.statefn)(self);
    }
}

fn state1(self_: &mut StateMachineIter) -> Option<&'static str> {
    self_.statefn = &id(state2 as StateMachineFunc);
// { dg-error ".E0716." "" { target *-*-* } .-1 }
    return Some("state1");
}

fn state2(self_: &mut StateMachineIter) -> Option<(&'static str)> {
    self_.statefn = &id(state3 as StateMachineFunc);
// { dg-error ".E0716." "" { target *-*-* } .-1 }
    return Some("state2");
}

fn state3(self_: &mut StateMachineIter) -> Option<(&'static str)> {
    self_.statefn = &id(finished as StateMachineFunc);
// { dg-error ".E0716." "" { target *-*-* } .-1 }
    return Some("state3");
}

fn finished(_: &mut StateMachineIter) -> Option<(&'static str)> {
    return None;
}

fn state_iter() -> StateMachineIter<'static> {
    StateMachineIter {
// { dg-error ".E0515." "" { target *-*-* } .-1 }
        statefn: &id(state1 as StateMachineFunc)
    }
}


fn main() {
    let mut it = state_iter();
    println!("{:?}",it.next());
    println!("{:?}",it.next());
    println!("{:?}",it.next());
    println!("{:?}",it.next());
    println!("{:?}",it.next());
}

