 #include <iostream> 

#include "testing.h"
#include "solution.h"

void Check(const IntVector& actual, const std::vector<int>& expected) {
    ASSERT_EQ(actual.Size(), expected.size());
    for (size_t i = 0; i < actual.Size(); ++i) {
        ASSERT_EQ(actual[i], expected[i]);
    }
}

void VectorHasConstructors() {
    {
        IntVector a;
        IntVector b;
        IntVector c;
        ASSERT_EQ(a.Size(), 0u);
        ASSERT_EQ(b.Size(), 0u);
        ASSERT_EQ(c.Size(), 0u);
    } 
    {
        IntVector a{1, 2, 3, 4};
        Check(a, std::vector<int>{1, 2, 3, 4});
    }
    {
        IntVector a(5);
        Check(a, std::vector<int>(5));
    }
}



void BasicMethods() {
    IntVector a{1, 3, 5};
    ASSERT_EQ(a.Capacity(), 3u);
    Check(a, std::vector<int>{1, 3, 5});

    a.PopBack();
    Check(a, std::vector<int>{1, 3});
    a.Clear();
    ASSERT_EQ(a.Capacity(), 3u);
    Check(a, std::vector<int>());

    a.PushBack(6);
    Check(a, std::vector<int>{6});

    IntVector b{3, 4};
    a.Swap(b);
    Check(a, std::vector<int>{3, 4});
    Check(b, std::vector<int>{6});
}


void IndexationAccess() {
    IntVector a{3, 7, 8};
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    Check(a, std::vector<int>{1, 2, 3});
}

void Reallocations() {
    const int steps = 16;
    IntVector data;
    for (int step = 0; step <= steps; ++step) {
        std::vector<int> ok_data;
        for (int i = 0; i <= (1 << step); ++i) {
            data.PushBack(i);
            ok_data.push_back(i);
        }
        Check(data, ok_data);
        ASSERT_EQ(data.Capacity(), 1u << (step + 1));
        data.Clear();
    }
}


void Reserve() {
    IntVector a;
    a.Reserve(5);
    ASSERT_EQ(a.Size(), 0u);
    ASSERT_EQ(a.Capacity(), 5u);

    for (int i = 0; i < 5; ++i) {
        a.PushBack(i);
    }

    ASSERT_EQ(a.Capacity(), 5u);
    a.PushBack(4);
    ASSERT_EQ(a.Capacity(), 10u);
    a.Reserve(3);
    ASSERT_EQ(a.Capacity(), 10u);
    a.Clear();
    ASSERT_EQ(a.Size(), 0u);
    ASSERT_EQ(a.Capacity(), 10u);
}


void CopyMoveCorrectness() {
    IntVector a;
    IntVector b(a);
    b.PushBack(1);
    Check(a, std::vector<int>());
    Check(b, std::vector<int>{1});

    b = b;
    Check(b, std::vector<int>{1});
    a = b;
    Check(a, std::vector<int>{1});

    b = std::move(a);
    Check(b, std::vector<int>{1});
    IntVector c(std::move(b));
    Check(c, std::vector<int>{1});

    IntVector d{3, 4, 5};
    IntVector e(d);
    Check(e, std::vector<int>{3, 4, 5});
    d.Swap(c);
    Check(e, std::vector<int>{3, 4, 5});
    Check(d, std::vector<int>{1});
    Check(c, std::vector<int>{3, 4, 5});
}

void RunAllPublicTests() {
    RUN_TEST(VectorHasConstructors);
    RUN_TEST(BasicMethods);
    RUN_TEST(IndexationAccess);
    RUN_TEST(Reallocations);
    RUN_TEST(Reserve);
    RUN_TEST(CopyMoveCorrectness);
}

int main() {
    StartTesting();
    RunAllPublicTests();
    
    return 0;
}
