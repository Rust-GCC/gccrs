// ignore-tidy-tab

fn main() {
	let some_vec = vec!["hi"];
	some_vec.into_iter();
	{
		println!("{:?}", some_vec); // { dg-error ".E0382." "" { target *-*-* } }
	}
}

