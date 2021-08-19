#include "parser.h"

static const std::string WHITESPACE = " \n\r\t\f\v";
static unsigned int linesParsed = 0;

static std::string ltrim(const std::string& s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}
static std::string rtrim(const std::string& s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
static std::string trim(const std::string& s)
{
    return rtrim(ltrim(s));
}
static std::vector<std::string> strip(const std::string& s, const std::string& del=" ")
{
    std::vector<std::string> ret;
    int start = 0;
    int end = s.find(del);
    while (end != -1)
    {
        ret.push_back(s.substr(start, end - start));
        start = end + del.size();
        end = s.find(del, start);
    }
    
    ret.push_back(s.substr(start, end - start));
    return ret;
}
static std::string replaceStringLiteral(std::string line)
{
    // String conv.
    bool inString = false;
    std::string tmpLine = "";
    for (int i = 0; i < line.size(); i++)
    {
        char c = line.at(i);
        if (c == '"')
        {
            inString = !inString;
        }
        else if (inString)
        {
            tmpLine.append(std::to_string((int)c));
            tmpLine.append((const char*)" ");
        }
        else
        {
            tmpLine.push_back(c);
        }
    }
    return tmpLine;
}
unsigned int getLinesParsed()
{
	return linesParsed;
}

ParsedLine parseLine(std::string line)
{
    ParsedLine ret;

    // Parsing the string.
    line = replaceStringLiteral(line);
    line = trim(line);

    if (line.size() == 0 || line.at(0) == ';')
    {
        ret.isEmpty = true;
    }
    else
    {
        ret.isEmpty = false;
        std::vector<std::string> lineSplit = strip(line);

        // Making return struct.
        ret.action = lineSplit.at(0);

        if (lineSplit.size() > 1)
        {
            ret.hasArgs = true;
            for (int i = 1; i < lineSplit.size(); i++)
            {
                std::string s = lineSplit.at(i);
                if (s.empty()) { continue; }

                int arg = atoi(s.c_str());
                if (arg == 0 && s == ".")
                {
                    ret.args.push_back(0);
                    ret.dotPos = i - 1;
                } 
                else if (arg == 0 && s != "0") 
                {
                    ret.hasError = true;
                    ret.errorMessage = "Actions only accepts integers.";
                }
                else
                {
                    ret.args.push_back(arg);
                }
            }
        }
        else
        {
            ret.hasArgs = false;
        }
    }

    linesParsed++;
    return ret;
}