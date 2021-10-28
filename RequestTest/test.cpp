#include "pch.h"
#include "../RecursiveRoundFileSystem/TreeRequests.h"
#include "../RecursiveRoundFileSystem/FileClass.h"


TEST(TestRequest, SimpleRequestLength) {
    File file("", "", 120, "");
    std::string requestOneCmpRigth = "Length > 20";
    std::string requestTwoCmp = "1 <= Length < 110";
    TreeNode* Root1 = nullptr;
    TreeNode* Root3 = nullptr;
    Add(requestOneCmpRigth, Root1);
    Add(requestTwoCmp, Root3);
    ASSERT_TRUE(preorderRound(Root1, file));
    ASSERT_FALSE(preorderRound(Root3, file));
}

TEST(TestRequest, SimpleRequestName) {
    File file("", "test.txt", 0, "");
    std::string str1 = "Name = 'test.txt'";
    std::string str2 = "Name ~ '.*\.txt'";
    std::string str3 = "Name ~ '.*\.doc'";
    TreeNode* Root1 = nullptr;
    TreeNode* Root2 = nullptr;
    TreeNode* Root3 = nullptr;
    Add(str1, Root1);
    Add(str2, Root2);
    Add(str3, Root3);
    ASSERT_TRUE(preorderRound(Root1, file));
    ASSERT_TRUE(preorderRound(Root2, file));
    ASSERT_FALSE(preorderRound(Root3, file));
}

TEST(TestRequest, SimpleRequestContent) {
    File file("", "", 0, "something content");
    std::string str1 = "Content = 'something content'";
    std::string str2 = "Content ~ '.*'";
    std::string str3 = "Content ~ '.*\.doc'";
    TreeNode* Root1 = nullptr;
    TreeNode* Root2 = nullptr;
    TreeNode* Root3 = nullptr;
    Add(str1, Root1);
    Add(str2, Root2);
    Add(str3, Root3);
    ASSERT_TRUE(preorderRound(Root1, file));
    ASSERT_TRUE(preorderRound(Root2, file));
    ASSERT_FALSE(preorderRound(Root3, file));
}

TEST(TestRequest, TestLogicAnd) {
    File file("", "test.txt", 120, "something content");
    std::string str1 = "(Content = 'something content') & (Length > 20) & (Name ~ '.*\.txt')";
    std::string str2 = "(Content = 'something content') & (Length > 20)";
    std::string str3 = "(Content = 'something content') & (Length > 20) & (Name ~ '.*\.doc')";
    TreeNode* Root1 = nullptr;
    TreeNode* Root2 = nullptr;
    TreeNode* Root3 = nullptr;
    Add(str1, Root1);
    Add(str2, Root2);
    Add(str3, Root3);
    ASSERT_TRUE(preorderRound(Root1, file));
    ASSERT_TRUE(preorderRound(Root2, file));
    ASSERT_FALSE(preorderRound(Root3, file));
}

TEST(TestRequest, TestLogicOr) {
    File file("", "test.txt", 120, "something content");
    std::string str1 = "(Content = 'something content') | (Length > 20) | (Name ~ '.*\.txt')";
    std::string str2 = "(Content = 'something content') | (Length > 20M)";
    std::string str3 = "(Content = '[0-9]*') | (Length > 20M) | (Name ~ '.*\.doc')";
    TreeNode* Root1 = nullptr;
    TreeNode* Root2 = nullptr;
    TreeNode* Root3 = nullptr;
    Add(str1, Root1);
    Add(str2, Root2);
    Add(str3, Root3);
    ASSERT_TRUE(preorderRound(Root1, file));
    ASSERT_TRUE(preorderRound(Root2, file));
    ASSERT_FALSE(preorderRound(Root3, file));
}

TEST(TestRequest, TestCompositionOrAnd) {
    File file("", "test.txt", 120, "something content");
    std::string str1 = "((Content = 'something content') | (Length > 20)) & ((Name ~ '.*\.txt') | (Length > 20G))";
    std::string str2 = "((Content = 'something content') & (Length > 20M)) | (Name ~ '.*\.txt')";
    std::string str3 = "(Content = '[0-9]*') | (Length > 20M) & (Name ~ '.*\.doc')";
    TreeNode* Root1 = nullptr;
    TreeNode* Root2 = nullptr;
    TreeNode* Root3 = nullptr;
    Add(str1, Root1);
    Add(str2, Root2);
    Add(str3, Root3);
    ASSERT_TRUE(preorderRound(Root1, file));
    ASSERT_TRUE(preorderRound(Root2, file));
    ASSERT_FALSE(preorderRound(Root3, file));
}

TEST(TestRequest, TestNot) {
    File file("", "test.txt", 120, "something content");
    std::string str1 = "^(((Content = 'something content') | (Length > 20)) & ((Name ~ '.*\.txt') | (Length > 20G)))";
    std::string str2 = "^((Content = 'something content') & (Length > 20M)) | (Name ~ '.*\.txt')";
    std::string str3 = "^Name ~ '.*\.doc'";
    TreeNode* Root1 = nullptr;
    TreeNode* Root2 = nullptr;
    TreeNode* Root3 = nullptr;
    Add(str1, Root1);
    Add(str2, Root2);
    Add(str3, Root3);
    ASSERT_FALSE(preorderRound(Root1, file));
    ASSERT_TRUE(preorderRound(Root2, file));
    ASSERT_TRUE(preorderRound(Root3, file));
}

TEST(TestRequest, ManyRequest) {
    File file("", "test.txt", 120, "something content");
    std::string str1 = "((10 < Length < 2M) & (^Name ~ '.*\\.doc')) & ((Content ~ '.*') | (Length > 10M))";
    std::string str2 = "^((Content = 'something content') | (Length > 10) | (Name ~ '.*\.txt'))";
    TreeNode* Root1 = nullptr;
    TreeNode* Root2 = nullptr;
    TreeNode* Root3 = nullptr;
    Add(str1, Root1);
    Add(str2, Root2);
    ASSERT_TRUE(preorderRound(Root1, file));
    ASSERT_FALSE(preorderRound(Root2, file));
}
