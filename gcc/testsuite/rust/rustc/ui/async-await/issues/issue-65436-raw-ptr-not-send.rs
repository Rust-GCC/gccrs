// edition:2018

struct Foo(*const u8);

unsafe impl Send for Foo {}

async fn bar(_: Foo) {}

fn assert_send<T: Send>(_: T) {}

fn main() {
    assert_send(async {
// { dg-error "" "" { target *-*-* } .-1 }
        bar(Foo(std::ptr::null())).await;
    })
}

