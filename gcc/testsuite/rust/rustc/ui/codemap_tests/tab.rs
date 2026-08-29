// ignore-tidy-tab

fn main() {
	bar; // { dg-error ".E0425." "" { target *-*-* } }
}

fn foo() {
	"bar			boo" // { dg-error ".E0308." "" { target *-*-* } }
}

