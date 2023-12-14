#include <iostream>
#include <string>
#include <map>

std::string trim(const std::string &str)
{
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos)
        return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

bool SetAllSessionVarsFromHeaders(const std::string &serverVars, std::map<std::string, std::string> &sessionVariables)
{
    size_t start = 0;
    size_t end = serverVars.find("\r\n", start);
    while (end != std::string::npos)
    {
        std::string line = serverVars.substr(start, end - start);

        if (line.rfind("X-siLock-SessVar", 0) == 0)
        {
            size_t firstColon = line.find(':', strlen("X-siLock-SessVar"));
            if (firstColon != std::string::npos)
            {
                size_t secondColon = line.find(':', firstColon + 1);
                if (secondColon != std::string::npos)
                {
                    std::string key = line.substr(firstColon + 2, secondColon - firstColon - 2);
                    std::string value = line.substr(secondColon + 1);

                    key = trim(key);
                    value = trim(value);

                    sessionVariables[key] = value;
                    std::cout << "Set " << key << " to " << value << std::endl;
                }
            }
        }

        start = end + 2;
        end = serverVars.find("\r\n", start);
    }

    return true;
}

int main()
{
    std::map<std::string, std::string> sessionVariables;
    std::string serverVars = "X-siLock-SessVar0: MyUsername: sysadmin\r\n";
    SetAllSessionVarsFromHeaders(serverVars, sessionVariables);

    for (const auto &pair : sessionVariables)
    {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    }

    return 0;
}
