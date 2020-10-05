#define CATCH_CONFIG_MAIN
//#include <catch.hpp>

#include <unordered_map>
#include <string>
#include <cmath>
using namespace std;

class Solution
{
public:
    int lengthOfLongestSubstring(string s)
    {
        int result = 0;
        const unordered_map<char, int> charMap;
        int i = 0, j = 0;

        while (j < s.length())
        {
            char c = s.at(j);
            auto itr = charMap.find(c);
            if (itr != charMap.end())
            {
                int preIndex = itr->second;
                for (int t = i; t < preIndex + 1; ++t)
                {
                    charMap.erase(s.at(t));
                }
                i = preIndex + 1;
            }
            else
            {
                charMap.insert(make_pair(c, j));
                ++j;
            }
            result = max(result, j - i);
        }

        return result;
    }
};