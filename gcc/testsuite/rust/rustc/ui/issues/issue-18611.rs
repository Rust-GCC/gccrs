fn add_state(op: <isize as HasState>::State) {
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

trait HasState {
    type State;
}

fn main() {}

