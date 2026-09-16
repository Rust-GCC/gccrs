fn f(x:isize) {
    static child: isize = x + 1;
// { dg-error ".E0435." "" { target *-*-* } .-1 }
}

fn main() {}

