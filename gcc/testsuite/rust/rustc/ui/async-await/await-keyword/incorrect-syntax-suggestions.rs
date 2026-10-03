// edition:2018

async fn bar() -> Result<(), ()> {
    Ok(())
}

async fn foo1() -> Result<(), ()> {
    let _ = await bar(); // { dg-error "" "" { target *-*-* } }
    Ok(())
}
async fn foo2() -> Result<(), ()> {
    let _ = await? bar(); // { dg-error "" "" { target *-*-* } }
    Ok(())
}
async fn foo3() -> Result<(), ()> {
    let _ = await bar()?; // { dg-error "" "" { target *-*-* } }
    Ok(())
}
async fn foo21() -> Result<(), ()> {
    let _ = await { bar() }; // { dg-error "" "" { target *-*-* } }
    Ok(())
}
async fn foo22() -> Result<(), ()> {
    let _ = await(bar()); // { dg-error "" "" { target *-*-* } }
    Ok(())
}
async fn foo23() -> Result<(), ()> {
    let _ = await { bar() }?; // { dg-error "" "" { target *-*-* } }
    Ok(())
}
async fn foo4() -> Result<(), ()> {
    let _ = (await bar())?; // { dg-error "" "" { target *-*-* } }
    Ok(())
}
async fn foo5() -> Result<(), ()> {
    let _ = bar().await(); // { dg-error "" "" { target *-*-* } }
    Ok(())
}
async fn foo6() -> Result<(), ()> {
    let _ = bar().await()?; // { dg-error "" "" { target *-*-* } }
    Ok(())
}
async fn foo7() -> Result<(), ()> {
    let _ = bar().await; // OK
    Ok(())
}
async fn foo8() -> Result<(), ()> {
    let _ = bar().await?; // OK
    Ok(())
}
fn foo9() -> Result<(), ()> {
    let _ = await bar(); // { dg-error ".E0728." "" { target *-*-* } }
// { dg-error ".E0728." "" { target *-*-* } .-1 }
    Ok(())
}
fn foo10() -> Result<(), ()> {
    let _ = await? bar(); // { dg-error ".E0728." "" { target *-*-* } }
// { dg-error ".E0728." "" { target *-*-* } .-1 }
    Ok(())
}
fn foo11() -> Result<(), ()> {
    let _ = await bar()?; // { dg-error "" "" { target *-*-* } }
    Ok(())
}
fn foo12() -> Result<(), ()> {
    let _ = (await bar())?; // { dg-error ".E0728." "" { target *-*-* } }
// { dg-error ".E0728." "" { target *-*-* } .-1 }
    Ok(())
}
fn foo13() -> Result<(), ()> {
    let _ = bar().await(); // { dg-error ".E0728." "" { target *-*-* } }
// { dg-error ".E0728." "" { target *-*-* } .-1 }
    Ok(())
}
fn foo14() -> Result<(), ()> {
    let _ = bar().await()?; // { dg-error ".E0728." "" { target *-*-* } }
// { dg-error ".E0728." "" { target *-*-* } .-1 }
    Ok(())
}
fn foo15() -> Result<(), ()> {
    let _ = bar().await; // { dg-error ".E0728." "" { target *-*-* } }
    Ok(())
}
fn foo16() -> Result<(), ()> {
    let _ = bar().await?; // { dg-error ".E0728." "" { target *-*-* } }
    Ok(())
}
fn foo24() -> Result<(), ()> {
    fn foo() -> Result<(), ()> {
        let _ = bar().await?; // { dg-error ".E0728." "" { target *-*-* } }
        Ok(())
    }
    foo()
}
fn foo25() -> Result<(), ()> {
    let foo = || {
        let _ = bar().await?; // { dg-error ".E0728." "" { target *-*-* } }
        Ok(())
    };
    foo()
}

async fn foo26() -> Result<(), ()> {
    let _ = await!(bar()); // { dg-error "" "" { target *-*-* } }
    Ok(())
}
async fn foo27() -> Result<(), ()> {
    let _ = await!(bar())?; // { dg-error "" "" { target *-*-* } }
    Ok(())
}
fn foo28() -> Result<(), ()> {
    fn foo() -> Result<(), ()> {
        let _ = await!(bar())?; // { dg-error ".E0728." "" { target *-*-* } }
// { dg-error ".E0728." "" { target *-*-* } .-1 }
        Ok(())
    }
    foo()
}
fn foo29() -> Result<(), ()> {
    let foo = || {
        let _ = await!(bar())?; // { dg-error ".E0728." "" { target *-*-* } }
// { dg-error ".E0728." "" { target *-*-* } .-1 }
        Ok(())
    };
    foo()
}

fn main() {
    match await { await => () }
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
} // { dg-error "" "" { target *-*-* } }

