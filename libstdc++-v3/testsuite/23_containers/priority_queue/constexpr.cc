// { dg-do compile { target c++26 } }
#include <queue>

constexpr bool test_constexpr_priority_queue() {
  std::priority_queue<int> pq;
  if (!pq.empty()) return false;
  if (pq.size() != 0) return false;

  pq.push(10);
  if (pq.empty()) return false;
  if (pq.size() != 1) return false;
  if (pq.top() != 10) return false;

  pq.emplace(30);
  pq.emplace(20);
  if (pq.size() != 3) return false;
  if (pq.top() != 30) return false;

  pq.pop();
  if (pq.size() != 2) return false;
  if (pq.top() != 20) return false;

  std::priority_queue<int> pq2;
  pq2 = pq;
  if (pq2.size() != 2) return false;
  if (pq2.top() != 20) return false;

  std::priority_queue<int> pq3;
  pq3.push(100);
  pq.swap(pq3);
  if (pq.size() != 1 || pq.top() != 100) return false;
  if (pq3.size() != 2 || pq3.top() != 20) return false;

  int arr[] = {50, 200, 10};
  pq.push_range(arr);
  if (pq.size() != 4) return false;
  if (pq.top() != 200) return false;

  return true;
}

static_assert(test_constexpr_priority_queue(),
              "constexpr priority_queue test failed!");
