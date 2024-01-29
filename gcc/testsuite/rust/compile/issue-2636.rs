pub mod module_a {
    const CONST_I32: i32 = 0;
	static BAR:i32 = CONST_I32;
}

fn main() {
    let a = module_a::CONST_I32;
    // { dg-error "definition is private in this context" "" { target *-*-* } .-1 }
	let b = module_a::BAR;
    // { dg-error "definition is private in this context" "" { target *-*-* } .-1 }
}
