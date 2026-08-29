// edition:2018

fn main() {}

async fn response(data: Vec<u8>) {
    data.reverse(); // { dg-error ".E0596." "" { target *-*-* } }
}

