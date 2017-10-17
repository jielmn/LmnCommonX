#include <Windows.h>
int Foo(int a, int b)
{
	if (a == 0 || b == 0)
	{
		throw "don't do that";
	}
	int c = a % b;
	if (c == 0)
		return b;
	return Foo(b, c);
}

#include <gtest/gtest.h>

TEST(FooTest, HandleNoneZeroInput)
{
	EXPECT_EQ(2, Foo(4, 10));
	EXPECT_EQ(6, Foo(30, 18));
}

TEST(abc, xyz)
{
	EXPECT_EQ(1, 1);
	EXPECT_EQ(6, 0);
}

TEST(a123, b456)
{
	EXPECT_EQ(1, 1);
	EXPECT_EQ(2, 2);
}

int main(int argc, TCHAR* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}