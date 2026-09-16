// edition:2018

async fn print_dur() {}

fn main() {
    async { let (); }.await;
// { dg-error ".E0728." "" { target *-*-* } .-1 }
    async {
// { dg-error ".E0728." "" { target *-*-* } .-1 }
        let task1 = print_dur().await;
    }.await;
    (|_| 2333).await;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
}

