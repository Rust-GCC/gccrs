fn main() {
    let _ = std::thread::thread_info::current_thread();
// { dg-error ".E0603." "" { target *-*-* } .-1 }
}

