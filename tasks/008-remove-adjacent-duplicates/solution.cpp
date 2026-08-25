#include <cassert>
#include <string>
#include <algorithm>

void removeAdjacentDuplicatesStd(std::string& text)
{
    text.erase(std::unique(text.begin(), text.end()), text.end());
}


void removeAdjacentDuplicates(std::string& text)
{
   size_t w = 0;
   size_t sz = text.size();
   if (sz < 2) return;
   for (size_t i = 1; i < sz; ++i) {
       if (text[i] != text[i-1])
           text[++w] = text[i];
   }
   text.resize(w+1);
}

int main()
{
    {
        std::string text = "aaabbcdddde";
        removeAdjacentDuplicates(text);
        assert(text == "abcde");
    }

    {
        std::string text = "a";
        removeAdjacentDuplicates(text);
        assert(text == "a");
    }

    {
        std::string text;
        removeAdjacentDuplicates(text);
        assert(text.empty());
    }

    {
        std::string text = "abcdef";
        removeAdjacentDuplicates(text);
        assert(text == "abcdef");
    }

    {
        std::string text = "aaaa";
        removeAdjacentDuplicates(text);
        assert(text == "a");
    }

    {
        std::string text = "aabbaa";
        removeAdjacentDuplicates(text);
        assert(text == "aba");
    }

    {
        std::string text = "1122332211";
        removeAdjacentDuplicates(text);
        assert(text == "12321");
    }

    return 0;
}
