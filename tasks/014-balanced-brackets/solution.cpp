#include <cassert>
#include <string_view>

bool hasBalancedBrackets(std::string_view text)
{
    return false;
}

int main()
{
    assert(hasBalancedBrackets(""));
    assert(hasBalancedBrackets("()"));
    assert(hasBalancedBrackets("[]"));
    assert(hasBalancedBrackets("{}"));
    assert(hasBalancedBrackets("([]){}"));
    assert(hasBalancedBrackets("([{}])"));
    assert(hasBalancedBrackets("((()))"));
    assert(hasBalancedBrackets("{}[]()"));

    assert(!hasBalancedBrackets("("));
    assert(!hasBalancedBrackets(")"));
    assert(!hasBalancedBrackets("([)]"));
    assert(!hasBalancedBrackets("{[}]"));
    assert(!hasBalancedBrackets("(()"));
    assert(!hasBalancedBrackets("())"));
    assert(!hasBalancedBrackets("[[[]"));

    return 0;
}
