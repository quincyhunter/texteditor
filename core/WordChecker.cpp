// WordChecker.cpp
//
// ICS 46 Spring 2022
// Project #4: Set the Controls for the Heart of the Sun
//
// Replace and/or augment the implementations below as needed to meet
// the requirements.

#include "WordChecker.hpp"
#include <algorithm>

WordChecker::WordChecker(const Set<std::string>& words)
    : words{words}
{
}


bool WordChecker::wordExists(const std::string& word) const
{
    return words.contains(word);
}


std::vector<std::string> WordChecker::findSuggestions(const std::string& word) const
{
    // Nonsensical code because the compiler requires the member variables
    // 'words' to be used somewhere, or else it becomes a warning (which
    // turns into an error).

//setup
std::vector<std::string> sl;
if (words.contains(word))
    return sl;

std::string tmp = word;
//swap
for(int i = 0; i < word.size();i++)
{
    std::swap(tmp[i],tmp[i+1]);
    if(words.contains(tmp))
    {
        if(std::find(sl.begin(),sl.end(),tmp) == sl.end())
            sl.push_back(tmp);
    }
tmp = word;
}


//insert
for(int i = 0; i<word.size()+1; i++)
{
    for(char j = 'A';j<='Z';j++)
    {
        tmp.insert(i,1,j);
        if (words.contains(tmp))
        {
            if(std::find(sl.begin(),sl.end(),tmp) == sl.end())
                sl.push_back(tmp);
        }
    tmp = word;
    }
}
   
//deleting
for(int i = 0; i<word.size();i++) 
{
    tmp.erase(i,1);
    if (words.contains(tmp))
    {
        if(std::find(sl.begin(),sl.end(),tmp) == sl.end())
            sl.push_back(tmp);
    }
    tmp = word;
}

//replacing
for(int i = 0; i<word.size(); i++)
{
    for(char j = 'A';j<='Z';j++)
    {
        tmp.erase(i,1);
        tmp.insert(i,1,j);
        if (words.contains(tmp))
        {
            if(std::find(sl.begin(),sl.end(),tmp) == sl.end())
                sl.push_back(tmp);
        }
    tmp = word;
    }
}
//splitting
for(int i = 0; i<word.size()+1; i++)
{
    char c  = ' ';
    tmp.insert(i,1,c);
    if (words.contains(tmp))
    {
        if(std::find(sl.begin(),sl.end(),tmp) == sl.end())
            sl.push_back(tmp);
    }
    tmp = word;
    
}

return sl;
}

