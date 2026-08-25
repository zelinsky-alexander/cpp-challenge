#include <cassert>
#include <string_view>
#include <stack>

bool hasBalancedBrackets(std::string_view text)
{
    std::stack<char> stk;
    for (size_t i = 0; i < text.size(); ++i) {
      char c = text[i];
      switch(c) {
        case '(':
        case '[':
        case '{':
          stk.push(c);
          break;
        case ')':
          if (stk.empty() || stk.top() != '(') return false;
          stk.pop();
          break;
        case ']':
          if (stk.empty() || stk.top () != '[') return false;
          stk.pop();
          break;
        case '}':
          if (stk.empty() || stk.top() != '{') return false;
          stk.pop();
  break;

        default:
          break;
       }
    }
    return stk.empty();
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
