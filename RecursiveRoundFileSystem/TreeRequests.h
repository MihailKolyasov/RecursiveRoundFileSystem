#include <iostream>
#include <string>
#include <regex>
#include <list>
#include "FileClass.h"
#include "Request.h"

struct  TreeNode
{
    std::string data; 
    TreeNode* left; 
    TreeNode* right; 
};

void Add(std::string __data, TreeNode*& __treeNode);
bool preorderRound(TreeNode* root, File& file);
bool ChekRequest(TreeNode* root);
