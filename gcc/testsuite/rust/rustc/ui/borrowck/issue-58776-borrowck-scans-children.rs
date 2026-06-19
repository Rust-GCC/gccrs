fn main() {
    let mut greeting = "Hello world!".to_string();
    let res = (|| (|| &greeting)())();

    greeting = "DEALLOCATED".to_string();
// { dg-error ".E0506." "" { target *-*-* } .-1 }
    drop(greeting);
// { dg-error ".E0505." "" { target *-*-* } .-1 }

    println!("thread result: {:?}", res);
}

