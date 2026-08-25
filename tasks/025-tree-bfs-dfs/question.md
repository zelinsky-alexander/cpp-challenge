# Task 025 — Tree BFS and DFS

## Goal

Given this binary-tree node type:

```cpp
struct TreeNode {
    int value;
    TreeNode* left;
    TreeNode* right;
};
```

Implement:

```cpp
std::vector<int> bfs(const TreeNode* root);
std::vector<int> dfsPreorder(const TreeNode* root);
```

`bfs` returns level-order traversal. `dfsPreorder` returns root-left-right preorder traversal.

## Example

```text
        1
       / \
      2   3
     / \
    4   5

bfs        -> {1, 2, 3, 4, 5}
dfsPreorder -> {1, 2, 4, 5, 3}
```

## Requirements

- Use C++20.
- Do not modify the tree.
- Return an empty vector for a null root.
- Visit every node exactly once per traversal.
- Target time complexity: `O(n)` for each traversal.

## Build and run

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -g \
  tasks/025-tree-bfs-dfs/solution.cpp -o /tmp/cpp-challenge-025
/tmp/cpp-challenge-025
```
