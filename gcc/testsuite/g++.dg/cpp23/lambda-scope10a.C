// PR c++/122884
// { dg-do compile { target c++23 } }

struct demux_t {
    template <class Fn, class... Slots>
    static constexpr auto operator()(Fn&& fn, Slots&&... slots) {
        return [fn{fn}, ... slots{slots}]<class Self, class... Args>(
                   this Self&&,
                   Args&&... args)
                   noexcept(noexcept(fn(slots(args...)...))) // comment this
                   -> decltype(auto)
	  requires (!noexcept (fn(slots(args...)...))) // and this, no longer crash
        { return fn(slots(args...)...); };
    }
} static constexpr demux{};

static_assert(demux([](auto x, auto y) { return x + y; },
                    [](auto...) { return 1; },
                    [](auto...) { return 2; })(1, 2) == 3);
