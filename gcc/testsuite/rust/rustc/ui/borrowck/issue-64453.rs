struct Project;
struct Value;

static settings_dir: String = format!("");
// { dg-error ".E0015." "" { target *-*-* } .-1 }
// { dg-error ".E0015." "" { target *-*-* } .-2 }

fn from_string(_: String) -> Value {
    Value
}
fn set_editor(_: Value) {}

fn main() {
    let settings_data = from_string(settings_dir);
// { dg-error ".E0507." "" { target *-*-* } .-1 }
    let args: i32 = 0;

    match args {
        ref x if x == &0 => set_editor(settings_data),
        ref x if x == &1 => set_editor(settings_data),
        _ => unimplemented!(),
    }
}

