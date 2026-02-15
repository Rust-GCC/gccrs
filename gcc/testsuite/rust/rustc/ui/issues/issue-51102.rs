enum SimpleEnum {
    NoState,
}

struct SimpleStruct {
    no_state_here: u64,
}

fn main() {
    let _ = |simple| {
        match simple {
            SimpleStruct {
                state: 0,
// { dg-error ".E0026." "" { target *-*-* } .-1 }
                ..
            } => (),
        }
    };

    let _ = |simple| {
        match simple {
            SimpleStruct {
                no_state_here: 0,
                no_state_here: 1
// { dg-error ".E0025." "" { target *-*-* } .-1 }
            } => (),
        }
    };

    let _ = |simple| {
        match simple {
            SimpleEnum::NoState {
                state: 0
// { dg-error ".E0026." "" { target *-*-* } .-1 }
            } => (),
        }
    };
}

