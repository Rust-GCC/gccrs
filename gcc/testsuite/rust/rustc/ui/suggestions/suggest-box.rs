// run-rustfix

fn main() {
    let _x: Box<dyn Fn() -> Result<(), ()>> = || { // { dg-error ".E0308." "" { target *-*-* } }
        Err(())?;
        Ok(())
    };
}

