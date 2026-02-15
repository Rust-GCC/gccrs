fn unrelated() -> Result<(), std::string::ParseError> {  // #57664
    let x = 0;

    match x {
        1 => {
            let property_value_as_string = "a".parse()?;
        }
        2 => {
            let value: &bool = unsafe { &42 };
// { dg-error ".E0308." "" { target *-*-* } .-1 }
        }
    };

    Ok(())
}

fn main() {}

