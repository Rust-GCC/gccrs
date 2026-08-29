fn main() {
    format!(r"{}{}{}", named_arg=1); // { dg-error "" "" { target *-*-* } }
}

