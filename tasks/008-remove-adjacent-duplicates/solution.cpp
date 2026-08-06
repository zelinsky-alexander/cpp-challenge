#include <cassert>
#include <string>

void removeAdjacentDuplicates(std::string& text)
{
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
