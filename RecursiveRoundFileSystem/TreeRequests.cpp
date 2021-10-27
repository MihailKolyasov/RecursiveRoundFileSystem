#include "TreeRequests.h"

std::string trim(const std::string& str, const std::string& whitespace = " \t")
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return "";

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

void Add(std::string __data, TreeNode*& __treeNode)
{
    TreeNode* ptr = new TreeNode;
    ptr->left = ptr->right = nullptr;
    ptr->data = __data;
    if (__data.find('&') != std::string::npos || __data.find('|') != std::string::npos)
    {
        std::string firstStr = "";
        std::string secondStr = "";
        std::list<char> logicList;
        std::regex regular("\\((.*)\\)([ |&]+?)\\((.*)\\)");
        std::cmatch result;
        if (__data.find(")") != std::string::npos)
        {
            while (__data.find('|') != std::string::npos || __data.find('&') != std::string::npos)
            {
                if (std::regex_match(__data.c_str(), result, regular))
                {
                    firstStr = result[1];
                    secondStr = result[3];
                    size_t countOpen = std::count(secondStr.begin(), secondStr.end(), '(');
                    size_t countClose = std::count(secondStr.begin(), secondStr.end(), ')');
                    if (countClose == countOpen)
                        break;
                    else
                    {
                        char del = trim(result[2])[0];
                        logicList.push_front(del);
                        __data = "(" + firstStr + ")" + "#(" + secondStr + ")";
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
        ptr->data = trim(result[2]);
        __treeNode = ptr;
        Add(firstStr, ptr->left);
        Add(secondStr, ptr->right);
    }
    else
    {
        __treeNode = ptr;
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
            return false;
        return true;
    }
    if (root->data == "|")
    {
        if (!preorderRound(root->left, file) && !preorderRound(root->right, file))
            return false;
        return true;
    }
    if (std::regex_match(root->data.c_str(), regularName))
    {
        std::regex nameRequest(GetNameRequest(root->data));
        if (std::regex_match(file.GetName().c_str(), nameRequest))
            return true;
    }
    if (std::regex_match(root->data.c_str(), regularContent))
    {
        std::regex contentRequest(GetContentRequest(root->data));
        if (std::regex_match(file.GetContent().c_str(), contentRequest))
            return true;
    }
    if (std::regex_match(root->data.c_str(), regularLen))
    { 
        unsigned long long* sizeArr = GetLengthRequest(root->data);
        unsigned long long size = file.GetSize();
        if (size >= sizeArr[0] && size <= sizeArr[1])
            return true;
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
