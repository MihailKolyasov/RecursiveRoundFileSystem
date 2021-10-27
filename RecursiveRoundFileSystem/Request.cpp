#include "Request.h"

unsigned long long int* GetLengthRequest(std::string size)
{
    unsigned long long int* arr = new unsigned long long int;
    std::cmatch result;

    std::regex regularLen("(([0-9]*)([KGM])*.*[<>= ]*)Length([<>= ]*([0-9]*)([KGM])*.*)");
 
    std::regex_match(size.c_str(), result, regularLen);
    unsigned long long int number;
    if (result[1] != "")
    {
        std::string leftBoard = result[1];
        std::stringstream sstream(result[2]);
        sstream >> number;
        if (result[3] == "K")
            number *= 1000;
        else
            if (result[3] == "M")
                number *= 1000000;
            else
                if (result[3] == "G")
                    number *= 1000000000;
        if (leftBoard.find("=") == std::string::npos)
            number += 1;
        *arr++ = number;
    }
    else
        *arr++ = 0;
    if (result[4] != "")
    {
        std::string rigthBoard = result[4];
        std::stringstream sstream(result[5]);
        sstream >> number;
        if (result[6] == "K")
            number *= 1000;
        else
            if (result[6] == "M")
                number *= 1000000;
            else
                if (result[6] == "G")
                    number *= 1000000000;
        if (rigthBoard.find("=") == std::string::npos)
            number += 1;
        if (rigthBoard.find(">") != std::string::npos)
        {
            arr--;
            *arr++ = number;
            *arr++ = ULLONG_MAX;
        }
        else
            *arr++ = number;
    }
    else
        *arr++ = ULLONG_MAX;
    return arr-=2;
}

std::string GetNameRequest(std::string name)
{
    std::regex regularName("Name[ =~]+'{1}(.*?[^']+)'{1}");
    std::cmatch result;
    std::regex_match(name.c_str(), result, regularName);
    return result[1];
}

std::string GetContentRequest(std::string content)
{
    std::regex regularContent("Content[ =~]+'{1}(.*?[^']+)'{1}");
    std::cmatch result;
    std::regex_match(content.c_str(), result, regularContent);
    return result[1];
}
