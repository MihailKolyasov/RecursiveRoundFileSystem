#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "FileClass.h"
#include "Request.h"
#include "TreeRequests.h"
#include "..\include\dirent.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

int tabs = 0;
void print(TreeNode* root)
{
    if (!root)
        return;
    tabs++;
    print(root->left);
    for (int i = 0; i < tabs; i++)
        std::cout << "  ";
    std::cout << "|" << root->data << "|" <<  std::endl;
    print(root->right);
    tabs--;
    return;
}

void scandir(const char* dirname, TreeNode* Root)
{
    DIR* dir;
    struct dirent* ent;
    std::cout << "Search in " << dirname << std::endl;
    if ((dir = opendir(dirname)) == NULL)
    {
        std::cout << "Do not open dir." << std::endl;
        return;
    }
    while ((ent = readdir(dir)) != NULL)
    {
        if (strcmp(ent->d_name, ".") && strcmp(ent->d_name, ".."))
        {
            char s[255];
            s[0] = '\0';
            strcat(s, dirname);
            strcat(s, ent->d_name);
            if (opendir(s) == NULL)
            {
                std::string path = dirname;
                std::string fileName = ent->d_name;
                path = path + fileName;
                File file(path);
                if (preorderRound(Root, file))
                {
                    std::cout << path << std::endl;
                }
            }
            else
            {
                strcat(s, "\\");
                scandir(s,Root);
            }
        }
    }
    if (closedir(dir) != 0)
    {
        std::cout << "Do not close dir" << std::endl;
    }
    return;
}



int main(int argc, char* argv[])
{
    if (argc > 3)
    {
        std::cout << "Invalid arguments. Enter -h for help!" << std::endl;
        return 0;
    }
    std::string request = argv[1];
    const char* dir = argv[2];
    if (request == "-h")
    {
        std::cout << "Enter request and path for dir. For example:" << std::endl;
        std::cout << "\"(Length<2G)&(Name=\'abc.txt\')|(Content~\'top.*s\')\" \"D:\\\"" << std::endl;
        return 0;
    }
    TreeNode* Root = nullptr;
    Add(request, Root);
    try
    {
        if (!ChekRequest(Root))
            throw "Invalid request";
    }
    catch (const char* exception)
    {
        std::cout << exception << std::endl;
        return -1;
    }
    scandir(dir, Root);
    return 0;
}



