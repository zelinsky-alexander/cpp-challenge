#include <cassert>
#include <vector>

struct TreeNode {
    int value;
    TreeNode* left;
    TreeNode* right;
};

std::vector<int> bfs(const TreeNode* root);
std::vector<int> dfsPreorder(const TreeNode* root);

int main()
{
    assert(bfs(nullptr).empty());
    assert(dfsPreorder(nullptr).empty());

    TreeNode n4{4, nullptr, nullptr};
    TreeNode n5{5, nullptr, nullptr};
    TreeNode n2{2, &n4, &n5};
    TreeNode n3{3, nullptr, nullptr};
    TreeNode n1{1, &n2, &n3};

    assert(bfs(&n1) == std::vector<int>({1, 2, 3, 4, 5}));
    assert(dfsPreorder(&n1) == std::vector<int>({1, 2, 4, 5, 3}));

    TreeNode only{9, nullptr, nullptr};
    assert(bfs(&only) == std::vector<int>{9});
    assert(dfsPreorder(&only) == std::vector<int>{9});

    return 0;
}
