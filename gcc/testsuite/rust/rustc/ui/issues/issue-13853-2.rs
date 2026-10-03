trait FromStructReader<'a> { }
trait ResponseHook {
     fn get(&self);
}
fn foo(res : Box<dyn ResponseHook>) { res.get } // { dg-error ".E0615." "" { target *-*-* } }
fn main() {}

