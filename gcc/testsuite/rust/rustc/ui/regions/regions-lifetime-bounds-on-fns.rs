fn a<'a, 'b:'a>(x: &mut &'a isize, y: &mut &'b isize) {
    // Note: this is legal because of the `'b:'a` declaration.
    *x = *y;
}

fn b<'a, 'b>(x: &mut &'a isize, y: &mut &'b isize) {
    // Illegal now because there is no `'b:'a` declaration.
    *x = *y; // { dg-error ".E0623." "" { target *-*-* } }
}

fn c<'a,'b>(x: &mut &'a isize, y: &mut &'b isize) {
    // Here we try to call `foo` but do not know that `'a` and `'b` are
    // related as required.
    a(x, y); // { dg-error ".E0623." "" { target *-*-* } }
}

fn d() {
    // 'a and 'b are early bound in the function `a` because they appear
    // inconstraints:
    let _: fn(&mut &isize, &mut &isize) = a; // { dg-error ".E0308." "" { target *-*-* } }
}

fn e() {
    // 'a and 'b are late bound in the function `b` because there are
    // no constraints:
    let _: fn(&mut &isize, &mut &isize) = b;
}

fn main() { }

