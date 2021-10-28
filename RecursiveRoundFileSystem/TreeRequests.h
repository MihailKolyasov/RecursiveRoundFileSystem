#include <iostream>
#include <string>
#include <regex>
#include <list>
#include "FileClass.h"
#include "Request.h"

struct  TreeNode
{
    std::string data;
    bool nagationFlag;
    TreeNode* left; 
    TreeNode* right; 
};

void Add(std::string data, TreeNode*& root, bool flag = false);
bool preorderRound(TreeNode* root, File& file);
bool ChekRequest(TreeNode* root);

std::string trim(const std::string& str, const std::string& whitespace = " \t")
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return "";
    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;
    return str.substr(strBegin, strRange);
}

void Add(std::string data, TreeNode*& root, bool flag)
{
    data = trim(data);
    TreeNode* ptr = new TreeNode;
    ptr->left = ptr->right = nullptr;
    ptr->data = data;
    ptr->nagationFlag = flag;
    if (data.find('&') != std::string::npos || data.find('|') != std::string::npos)
    {
        std::string firstStr = "";
        std::string secondStr = "";
        size_t countOpen = 0, countClose = 0;
        bool firstFlag = false, secondFlag = false;
        std::list<char> logicList;
        std::regex regular("(\\^)*\\((.*)\\)([ |&]+?)(\\^)*\\((.*)\\)");
        std::cmatch result;
        if (data.find(")") != std::string::npos)
        {
            if (data[0] == '^')
            {
                const auto strEnd = data.length();
                const auto strRange = strEnd - 3;
                std::string temp = data.substr(2, strRange);
                if (std::regex_match(temp.c_str(), regular))
                {
                    ptr->nagationFlag = true;
                    data = temp;
                }
             }
            while (data.find('|') != std::string::npos || data.find('&') != std::string::npos)
            {
                if (std::regex_match(data.c_str(), result, regular))
                {
                    if (result[1] == "^")
                        firstFlag = true;
                    if (result[4] == "^")
                        secondFlag = true;
                    firstStr = result[2];
                    secondStr = result[5];
                    countOpen = std::count(secondStr.begin(), secondStr.end(), '(');
                    countClose = std::count(secondStr.begin(), secondStr.end(), ')');
                    if (countClose == countOpen)
                        break;
                    else
                    {
                        char del = trim(result[3])[0];
                        logicList.push_front(del);
                        data = "(" + firstStr + ")" + "#(" + secondStr + ")";
                    }
                }
                else
                    break;
            }
        }

        for (auto i = logicList.begin(); i != logicList.end(); ++i)
        {
            secondStr[secondStr.find_first_of('#')] = *i;
        }
        if ((firstStr.find("|") != std::string::npos || firstStr.find("&") != std::string::npos) && std::count(firstStr.begin(), firstStr.end(), '(') % 2 == 1)
            firstStr = "(" + firstStr + ")";
        if ((secondStr.find("|") != std::string::npos || secondStr.find("&") != std::string::npos) && std::count(secondStr.begin(), secondStr.end(), '(') % 2 == 1)
            secondStr = "(" + secondStr + ")";
        ptr->data = trim(result[3]);
        root = ptr;
        Add(firstStr, ptr->left, firstFlag);
        Add(secondStr, ptr->right, secondFlag);
    }
    else
    {
        if (data[0] == '^')
        {
            ptr->nagationFlag = true;
            ptr->data = data.substr(1, data.length() - 1);
        }
        root = ptr;
    }
}


bool preorderRound(TreeNode* root, File& file)
{
    std::regex regularLen("(([0-9]*)([KGM ])*[<>= ]*)Length([<>= ]*([0-9]*)([KGM ])*)");
    std::regex regularName("Name[ =~]+'{1}(.*?[^']+)'{1}");
    std::regex regularContent("Content[ =~]+'{1}(.*?[^']+)'{1}");
    if (root == NULL)
    {
        return false;
    }
    if (root->data == "&")
    {
        if (!preorderRound(root->left, file) || !preorderRound(root->right, file))
        {
            if (root->nagationFlag)
                return true;
            else
                return false;
        }
        if (root->nagationFlag)
            return false;
        else
            return true;
    }
    if (root->data == "|")
    {
        if (!preorderRound(root->left, file) && !preorderRound(root->right, file))
        {
            if (root->nagationFlag)
                return true;
            else
                return false;
        }
        if (root->nagationFlag)
            return false;
        else
            return true;
    }
    
    if (std::regex_match(root->data.c_str(), regularName))
    {
        std::regex nameRequest(GetNameRequest(root->data));
        if (std::regex_match(file.GetName().c_str(), nameRequest))
            if (root->nagationFlag)
                return false;
            else
                return true;
        else
            if (root->nagationFlag)
                return true;
            else
                return false;
    }
    if (std::regex_match(root->data.c_str(), regularContent))
    {
        std::regex contentRequest(GetContentRequest(root->data));
        if (std::regex_match(file.GetContent().c_str(), contentRequest))
            if (root->nagationFlag)
                return false;
            else
                return true;
        else
            if (root->nagationFlag)
                return true;
            else
                return false;
    }
    if (std::regex_match(root->data.c_str(), regularLen))
    {
        unsigned long long* sizeArr = GetLengthRequest(root->data);
        unsigned long long size = file.GetSize();
        if (size >= sizeArr[0] && size <= sizeArr[1])
            if (root->nagationFlag)
                return false;
            else
                return true;
        else
            if (root->nagationFlag)
                return true;
            else
                return false;
    }
    return false;
}

bool ChekRequest(TreeNode* root)
{
    std::regex regularLen("(([0-9]*)([KGM ])*[<>= ]*)Length([<>= ]*([0-9]*)([KGM ])*)");
    std::regex regularName("Name[ =~]+'{1}(.*?[^']+)'{1}");
    std::regex regularContent("Content[ =~]+'{1}(.*?[^']+)'{1}");
    if (root == NULL)
    {
        return false;
    }
    if (root->data == "&" || root->data == "|")
    {
        return ChekRequest(root->left);
        return ChekRequest(root->right);
    }
    if (std::regex_match(root->data.c_str(), regularName))
    {
        return true;
    }
    if (std::regex_match(root->data.c_str(), regularContent))
    {
        return true;
    }
    if (std::regex_match(root->data.c_str(), regularLen))
    {
        return true;
    }
    return false;
}

