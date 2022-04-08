// AVLBagTests.cpp
#include <string>
#include <vector>
#include <gtest/gtest.h>
#include "AVLBag.hpp"


TEST(AVLBagTests, canCreateAndDestroy)
{
    AVLBag<int> s1;
    AVLBag<std::string> s2;
}


TEST(AVLBagTests, canCreateAndCleanup)
{
    AVLBag<int> s1;
    AVLBag<std::string> s2;
    s1.add(10);
    s2.add("stuff");
}


TEST(AVLBagTests, heightOfEmptyIsNegativeOne)
{
    AVLBag<int> s;
    ASSERT_EQ(-1, s.height());
}


TEST(AVLBagTests, canCopyConstructToCompatibleType)
{
    AVLBag<int> s1;
    AVLBag<std::string> s2;

    AVLBag<int> s1Copy{s1};
    AVLBag<std::string> s2Copy{s2};
}


TEST(AVLBagTests, canCopyConstruct)
{
    AVLBag<int> s1;
    for (int x = 0; x < 100; ++x)
    {
        s1.add(x);
    }
    AVLBag<int> s1Copy{s1};
    for (int x = 0; x < 100; ++x)
    {
        ASSERT_TRUE(s1Copy.contains(x));
    }
    ASSERT_EQ(s1.height(), s1Copy.height());
    ASSERT_EQ(s1.size(), s1Copy.size());
    
    AVLBag<std::string> s2;
    s2.add("a");
    s2.add("b");
    s2.add("c");
    s2.add("d");
    AVLBag<std::string> s2Copy{s2};
    ASSERT_TRUE(s2Copy.contains("a"));
    ASSERT_TRUE(s2Copy.contains("b"));
    ASSERT_TRUE(s2Copy.contains("c"));
    ASSERT_TRUE(s2Copy.contains("d"));
    ASSERT_EQ(s2.height(), s2Copy.height());
    ASSERT_EQ(s2.size(), s2Copy.size());
}


TEST(AVLBagTests, canMoveConstructToCompatibleType)
{
    AVLBag<int> s1;
    AVLBag<std::string> s2;

    AVLBag<int> s1Copy{std::move(s1)};
    AVLBag<std::string> s2Copy{std::move(s2)};
}


TEST(AVLBagTests, canMoveConstruct)
{
    AVLBag<int> s1;
    for (int x = 0; x < 100; ++x)
    {
        s1.add(x);
    }
    AVLBag<int> s1Copy{std::move(s1)};
    for (int x = 0; x < 100; ++x)
    {
        ASSERT_TRUE(s1Copy.contains(x));
    }
    ASSERT_EQ(100, s1Copy.size());
    
    AVLBag<std::string> s2;
    s2.add("a");
    s2.add("b");
    s2.add("c");
    s2.add("d");
    AVLBag<std::string> s2Copy{std::move(s2)};
    ASSERT_TRUE(s2Copy.contains("a"));
    ASSERT_TRUE(s2Copy.contains("b"));
    ASSERT_TRUE(s2Copy.contains("c"));
    ASSERT_TRUE(s2Copy.contains("d"));
    ASSERT_EQ(4, s2Copy.size());
}


TEST(AVLBagTests, canAssignToCompatibleType)
{
    AVLBag<int> s1;
    AVLBag<std::string> s2;

    AVLBag<int> s3;
    AVLBag<std::string> s4;

    s1 = s3;
    s2 = s4;
}


TEST(AVLBagTests, canAssignConstruct)
{
    AVLBag<int> s1;
    AVLBag<int> s1Copy;
    for (int x = 0; x < 50; ++x)
    {
        s1.add(x);
    }
    for (int x = -100; x < 0; ++x)
    {
        s1Copy.add(x);
    }
    s1Copy = s1;
    s1.add(50);
    s1.add(51);
    for (int x = 0; x < 50; ++x)
    {
        ASSERT_TRUE(s1Copy.contains(x));
    }
    for (int x = -100; x < 0; ++x)
    {
        ASSERT_FALSE(s1Copy.contains(x));
    }
    ASSERT_EQ(50, s1Copy.size());
    
    AVLBag<std::string> s2;
    AVLBag<std::string> s2Copy;
    s2.add("a");
    s2.add("b");
    s2Copy = s2;
    s2.add("c");
    s2.add("d");
    ASSERT_TRUE(s2Copy.contains("a"));
    ASSERT_TRUE(s2Copy.contains("b"));
    ASSERT_FALSE(s2Copy.contains("c"));
    ASSERT_FALSE(s2Copy.contains("d"));
    ASSERT_EQ(2, s2Copy.size());
}



TEST(AVLBagTests, canMoveAssignToCompatibleType)
{
    AVLBag<int> s1;
    AVLBag<std::string> s2;

    AVLBag<int> s3;
    AVLBag<std::string> s4;

    s1 = std::move(s3);
    s2 = std::move(s4);
}


TEST(AVLBagTests, containsElementsAfterAdding)
{
    AVLBag<int> s1;
    for (int x = 0; x < 100; ++x)
    {
        s1.add(x);
    }
    for (int x = 0; x < 100; ++x)
    {
        ASSERT_TRUE(s1.contains(x));
    }
    ASSERT_FALSE(s1.contains(101));
    ASSERT_FALSE(s1.contains(-1));
}


TEST(AVLBagTests, doesNotContainElementsNotAdded)
{
    AVLBag<int> s1;
    for (int x = 0; x < 50; ++x)
    {
        s1.add(x);
    }
    for (int x = 51; x < 100; ++x)
    {
        ASSERT_FALSE(s1.contains(x));
    }
}


TEST(AVLBagTests, canAddDuplicates)
{
    AVLBag<std::string> s0;
    s0.add("origin");
    s0.add("origin");
    ASSERT_EQ(s0.amount("origin"), 2);
    ASSERT_EQ(s0.amount("false"), 0);

    AVLBag<int> s1;
    for (int x = 0; x < 10; ++x)
    {
        s1.add(x);
    }
    for (int x = 0; x < 10; ++x)
    {
        s1.add(x);
    }
    ASSERT_EQ(3, s1.height());
    ASSERT_EQ(10, s1.size());
    for (int x = 0; x < 10; ++x)
    {
        ASSERT_EQ(s1.amount(x), 2);
    }
    AVLBag<int> s2;
    for (int x = 0; x < 100; ++x)
    {
        s2.add(x);
    }
    for (int y = 0; y < 4; ++y)
    {
        for (int x = 0; x < 100; ++x)
        {
            s2.add(x);
        }
    }
    ASSERT_EQ(100, s2.size());
    for (int x = 0; x < 100; ++x)
    {
        ASSERT_EQ(s2.amount(x), 5);
    }
}


TEST(AVLBagTests, sizeIsNumberOfElementsAdded)
{
    AVLBag<int> s1;
    s1.add(11);
    s1.add(1);
    s1.add(5);

    EXPECT_EQ(3, s1.size());
}


TEST(AVLBagTests, heightDependsOnBalancing)
{
    AVLBag<int> balanced{};
    balanced.add(1);
    balanced.add(2);
    balanced.add(3);
    balanced.add(4);

    EXPECT_EQ(2, balanced.height());
}


TEST(AVLBagTests, preOrderTraversal)
{
    AVLBag<int> s1;
    for (int x = 0; x < 10; ++x)
    {
        s1.add(x);
    }
    std::vector<int> preElements;
    s1.preorder([&](const int& element) { preElements.push_back(element); });
    std::vector<int> expectedPreElements{3, 1, 0, 2, 7, 5, 4, 6, 8, 9};

    for (unsigned int i = 0; i < 9; ++i)
    {
        EXPECT_EQ(preElements[i], expectedPreElements[i]);
    }
}


TEST(AVLBagTests, inOrderTraversal)
{
    AVLBag<int> s1;
    for (int x = 0; x < 10; ++x)
    {
        s1.add(x);
    }
    std::vector<int> inElements;
    s1.inorder([&](const int& element) { inElements.push_back(element); });
    std::vector<int> expectedInElements{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    for (unsigned int i = 0; i < 9; ++i)
    {
        EXPECT_EQ(inElements[i], expectedInElements[i]);
    }
}


TEST(AVLBagTests, postOrderTraversal)
{
    AVLBag<int> s1;
    for (int x = 0; x < 10; ++x)
    {
        s1.add(x);
    }
    std::vector<int> postElements;
    s1.postorder([&](const int& element) { postElements.push_back(element); });
    std::vector<int> expectedPostElements{0, 2, 1, 4, 6, 5, 9, 8, 7, 3};

    for (unsigned int i = 0; i < 9; ++i)
    {
        EXPECT_EQ(postElements[i], expectedPostElements[i]);
    }
}


TEST(AVLBagTests, speedTest)
{
    AVLBag<int> s1{};
    for (int x = 0; x < 100; ++x)
    {
        s1.add(x);
    }
}

