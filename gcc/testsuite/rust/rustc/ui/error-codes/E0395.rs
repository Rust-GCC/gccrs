static FOO: i32 = 42;
static BAR: i32 = 42;

static BAZ: bool = unsafe { (&FOO as *const i32) == (&BAR as *const i32) };
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {
}

