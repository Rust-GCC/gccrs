fn main() {
    let x = 0.0;
    let a = {
        loop {
            let c = if x > 0.0 {
                break 3;
            } else if x < 0.0 {
                x
            } else {
                return;
            };
            if x == 0.0 {
                break c; // { dg-error "expected .<integer>. got .<float>." }
            } else {
                continue;
            }
            return; // { dg-warning "unreachable statement" }
        }
    };

    let a = 1 + return; // { dg-error "cannot apply this operator to types <integer> and !" }
    // { dg-error "failed to type resolve expression" "" { target { *-*-* } } .-1 }
}
