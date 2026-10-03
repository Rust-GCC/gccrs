#![feature(try_trait, async_closure)]
// edition:2018
fn main() {}

async fn an_async_block() -> u32 {
    async {
        let x: Option<u32> = None;
        x?; // { dg-error ".E0277." "" { target *-*-* } }
        22
    }
    .await
}

async fn async_closure_containing_fn() -> u32 {
    let async_closure = async || {
        let x: Option<u32> = None;
        x?; // { dg-error ".E0277." "" { target *-*-* } }
        22_u32
    };

    async_closure().await
}

async fn an_async_function() -> u32 {
    let x: Option<u32> = None;
    x?; // { dg-error ".E0277." "" { target *-*-* } }
    22
}

