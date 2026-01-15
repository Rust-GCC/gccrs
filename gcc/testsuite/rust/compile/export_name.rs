#[export_name = "other_name"]
fn func() {}
// { dg-final { scan-assembler "other_name" } }

fn main() {
    func();
}
