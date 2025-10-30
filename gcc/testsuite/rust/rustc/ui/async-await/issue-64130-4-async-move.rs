// edition:2018
use std::any::Any;
use std::future::Future;

struct Client(Box<dyn Any + Send>);

impl Client {
    fn status(&self) -> u16 {
        200
    }
}

async fn get() { }

pub fn foo() -> impl Future + Send {
// { dg-error "" "" { target *-*-* } .-1 }
    let client = Client(Box::new(true));
    async move {
        match client.status() {
            200 => {
                let _x = get().await;
            },
            _ => (),
        }
    }
}

fn main() {}

