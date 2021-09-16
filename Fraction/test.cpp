#include <iostream>
#include <sstream>      
#include <string> 

#include "testing.h"
#include "solution.h"


void CtorDtor() {
    try {
        {
            Fraction a;
        }
        {
            Fraction a(13);
        }
        {
            Fraction a(7, 2);
        }
        {
            Fraction a = Fraction(1, 2);
        }
        {
            Fraction a(0, 1);
        }
        {
            Fraction a(0);
        }
    } catch(...) {
        std::cerr << "Ctor/Dtor have to be nothrow with correct values\n";
        throw;
    }

    bool failed = false;
    try {
        Fraction(1, 0);
    } catch (...) {
        failed = true;
    }
    if (!failed) {
        std::cerr << "The case of the zero denominator does not throw\n";
        throw std::runtime_error("Check failed");
    }
}

void CtorDtorCorrectness() {
    Fraction a;
    Fraction b(7, 12);
    Fraction c(3);
    Fraction d(8, 2);
    ASSERT_EQ(0, a.num());
    ASSERT_TRUE(a.den() != 0);
    ASSERT_EQ(7, b.num());
    ASSERT_EQ(12, b.den());
    ASSERT_EQ(3, c.num());
    ASSERT_EQ(1, c.den());
    ASSERT_EQ(4, d.num());
    ASSERT_EQ(1, d.den());
}

void CopyCtor() {
    Fraction a(2, 4);
    Fraction b(a);
    ASSERT_EQ(1, b.num());
    ASSERT_EQ(2, b.den());
}

void AssignmentOperator() {
    {
        Fraction a(6, 2);
        Fraction b;
        b = a;
        ASSERT_EQ(3, b.num());
        ASSERT_EQ(1, b.den());
    }
    {
        Fraction a(7, 2);
        Fraction b(1, 4);
        b = a;
        ASSERT_EQ(7, b.num());
        ASSERT_EQ(2, b.den());
    }
}

void OutputRepresentation() {
    std::stringbuf buffer;
    std::ostream os(&buffer);
    Fraction a(7, 77);
    os << a;
    ASSERT_TRUE(os.good());
    ASSERT_EQ("1/11", buffer.str());
}

void ComparisonOperators() {
    Fraction half(1, 2);
    Fraction one(1);
    Fraction another_one(1, 1);
    Fraction three(3);
    Fraction one_third(1, 3);
    Fraction negative(-5, 6);
    Fraction positive(5, 6);
    ASSERT_TRUE(half < one);
    ASSERT_TRUE(half != one);
    ASSERT_TRUE(half > one_third);
    ASSERT_TRUE(another_one == one);
    ASSERT_TRUE(three > one);
    ASSERT_TRUE(three >= one_third);
    ASSERT_TRUE(one_third <= positive);
    ASSERT_TRUE(one <= another_one);
    ASSERT_TRUE(one >= another_one);
    ASSERT_TRUE(positive > negative);
    ASSERT_TRUE(positive > 0);
    ASSERT_TRUE(5 > half);
    ASSERT_TRUE(positive < 1);
    ASSERT_TRUE(0 < positive);
    ASSERT_TRUE(one == 1);
    ASSERT_TRUE(1 == another_one);
    ASSERT_TRUE(1001 != negative);
    ASSERT_TRUE(one_third != 10001);
    ASSERT_TRUE(1 <= one);
    ASSERT_TRUE(3 >= three);
    ASSERT_TRUE(one_third <= 7);
    ASSERT_TRUE(half >= 0);
}

void UnaryOperators() {
    {
        Fraction a(1, 2);
        Fraction minusa(-1, 2);
        ASSERT_EQ(a, +a);
        ASSERT_EQ(minusa, -a);
    }
    {
        Fraction a(3, 4);
        ASSERT_EQ(Fraction(7, 4), ++a);
        ASSERT_EQ(Fraction(7, 4), a);
        ASSERT_EQ(Fraction(3, 4), --a);
        ASSERT_EQ(Fraction(3, 4), a);
        ASSERT_EQ(Fraction(3, 4), a++);
        ASSERT_EQ(Fraction(7, 4), a);
        ASSERT_EQ(Fraction(7, 4), a--);
        ASSERT_EQ(Fraction(3, 4), a);
    }
}

void BinaryOperators() {
   { 
       ASSERT_EQ(1, Fraction(1, 2) + Fraction(1, 2));
       ASSERT_EQ(Fraction(8, 7), 1 + Fraction(1, 7));
       ASSERT_EQ(Fraction(15, 7), Fraction(1, 7) + 2);
       ASSERT_EQ(-2, Fraction(-3, 2) - Fraction(1, 2));
       ASSERT_EQ(Fraction(1, 9), 1 - Fraction(8, 9));
       ASSERT_EQ(Fraction(4, 7), Fraction(18, 7) - 2);
       ASSERT_EQ(Fraction(3, 8), Fraction(3, 2) * Fraction(1, 4));
       ASSERT_EQ(Fraction(1, 2), Fraction(1, 4) * 2);
       ASSERT_EQ(Fraction(1, 2), 2 * Fraction(1, 4));
       ASSERT_EQ(Fraction(7, 4), Fraction(7, 18) / Fraction(4, 18));
       ASSERT_EQ(Fraction(1, 2), 1 / Fraction(2));
       ASSERT_EQ(Fraction(3, 4), Fraction(3, 2) / 2);
       ASSERT_EQ(0, Fraction(78971, 29790) * 0);
   }
   {
        Fraction a(15, 2);
        ASSERT_EQ(Fraction(19, 2), a += 2);
        ASSERT_EQ(Fraction(19, 2), a);
        ASSERT_EQ(Fraction(81, 10), a -= Fraction(7, 5));
        ASSERT_EQ(Fraction(81, 10), a);
        ASSERT_EQ(81, a *= 10);
        ASSERT_EQ(81, a);
        ASSERT_EQ(Fraction(1, 8), a /= 648);
        ASSERT_EQ(Fraction(1, 8), a);
   }
}

void ComplexTest() {
    Fraction a(3, 4);
    Fraction b(7, 9);
    Fraction c(13 ,8);
    ASSERT_EQ(Fraction(1, 117), ((a * b * c) / 91) / a / c);
    ASSERT_EQ(Fraction(7, 4), (c = b = ++a, b--));
    ASSERT_EQ(Fraction(3, 4), b);
    ASSERT_EQ(Fraction(7, 4), c);
}


void RunAllPublicTests() {
    RUN_TEST(CtorDtor);
    RUN_TEST(CtorDtorCorrectness);
    RUN_TEST(CopyCtor);
    RUN_TEST(AssignmentOperator);
    RUN_TEST(OutputRepresentation);
    RUN_TEST(ComparisonOperators);
    RUN_TEST(UnaryOperators);
    RUN_TEST(BinaryOperators);
    RUN_TEST(ComplexTest);
}

int main() {
    StartTesting();
    RunAllPublicTests();
    return 0;
}
