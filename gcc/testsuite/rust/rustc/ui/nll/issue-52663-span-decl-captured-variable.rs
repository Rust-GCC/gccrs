fn expect_fn<F>(f: F) where F : Fn() {
    f();
}

fn main() {
   {
       let x = (vec![22], vec![44]);
       expect_fn(|| drop(x.0));
// { dg-error ".E0507." "" { target *-*-* } .-1 }
   }
}

