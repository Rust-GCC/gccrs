// PR c++/125022
// { dg-do compile { target c++11 } }

struct raw_ptr {
  raw_ptr(int *);
};
struct [[clang::trivial_abi]] scoped_refptr {
  ~scoped_refptr();
  template <typename...> void emplace(scoped_refptr) {}
} main_thread_, AsyncMemoryConsumerRegistration_main_task_runner;
struct AsyncMemoryConsumerRegistration {
  AsyncMemoryConsumerRegistration();
  raw_ptr consumer_;
};
int AsyncMemoryConsumerRegistration_consumer;
AsyncMemoryConsumerRegistration::AsyncMemoryConsumerRegistration()
    : consumer_(&AsyncMemoryConsumerRegistration_consumer) {
  main_thread_.emplace(AsyncMemoryConsumerRegistration_main_task_runner);
}
