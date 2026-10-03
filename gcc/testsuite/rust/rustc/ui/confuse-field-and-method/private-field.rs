pub mod animal {
    pub struct Dog {
        pub age: usize,
        dog_age: usize,
    }

    impl Dog {
        pub fn new(age: usize) -> Dog {
            Dog { age: age, dog_age: age * 7 }
        }
    }
}

fn main() {
    let dog = animal::Dog::new(3);
    let dog_age = dog.dog_age(); // { dg-error ".E0599." "" { target *-*-* } }
    //let dog_age = dog.dog_age;
    println!("{}", dog_age);
}

