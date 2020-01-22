#include "gtest/gtest.h"
#include "Dummyclass.h"
#include <memory>

using namespace std;

TEST(suite1, methodtest1) {
    unique_ptr<Dummyclass> dummy(new Dummyclass(10,15));
    EXPECT_EQ(25,dummy->getT1()+dummy->getT2());
}

TEST(suite1, methodtest2) {
    unique_ptr<Dummyclass> dummy(new Dummyclass(10,15));
    EXPECT_EQ(5,dummy->dummymethod2(2));
}