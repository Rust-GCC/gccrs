// PR c++/105667
// { dg-do compile { target c++20 } }

struct class1
{
	virtual void a_function() = 0;
};

template<auto my_lambda = []<typename T>() {}>
class class2 {};

template<typename Touter>
struct class3 : public class1 {
	void a_function()
	{
		class2<> x;
	}
};

struct class4 : public class3<class4> {
	class4() {}
};
