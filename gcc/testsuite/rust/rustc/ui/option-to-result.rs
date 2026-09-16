fn main(){ }

fn test_result() -> Result<(),()> {
    let a:Option<()> = Some(());
    a?;// { dg-error ".E0277." "" { target *-*-* } }
    Ok(())
}

fn test_option() -> Option<i32>{
    let a:Result<i32, i32> = Ok(5);
    a?;// { dg-error ".E0277." "" { target *-*-* } }
    Some(5)
}

