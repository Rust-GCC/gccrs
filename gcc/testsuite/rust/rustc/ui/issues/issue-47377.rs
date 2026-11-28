// ignore-tidy-tab
fn main() {
 	let b = "hello";
 	let _a = b + ", World!";
// { dg-error ".E0369." "" { target *-*-* } .-1 }
}

