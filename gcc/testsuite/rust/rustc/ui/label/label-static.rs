fn main() {
    'static: loop { // { dg-error "" "" { target *-*-* } }
        break 'static // { dg-error "" "" { target *-*-* } }
    }
}

