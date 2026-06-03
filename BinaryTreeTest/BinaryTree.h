#pragma once

#include <climits>
#include <queue>
#include <stack>
#include <vector>

namespace BinaryTreeTest {

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

/**
 * 1. 二叉树中序遍历
 * 难度：简单
 * 思路：用栈模拟递归，按“左、根、右”的顺序访问。
 * 复杂度：时间 O(N)，空间 O(H)
 */
std::vector<int> inorderTraversal(TreeNode* root)
{
    std::vector<int> result;
    // 沿左链向下走时，栈中保存沿途祖先节点。
    std::stack<TreeNode*> st;
    TreeNode* current = root;

    while (current || !st.empty()) {
        // 中序遍历先访问左子树。
        while (current) {
            st.push(current);
            current = current->left;
        }
        // 栈顶节点就是下一个应访问的中序节点。
        current = st.top();
        st.pop();
        result.push_back(current->val);
        // 访问根节点后，再处理右子树。
        current = current->right;
    }
    return result;
}

/**
 * 2. 二叉树层序遍历
 * 难度：中等
 * 思路：用队列做 BFS，每轮处理一整层。
 * 复杂度：时间 O(N)，空间 O(N)
 */
std::vector<std::vector<int>> levelOrder(TreeNode* root)
{
    std::vector<std::vector<int>> result;
    if (!root) return result;

    std::queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        // 固定当前层节点数量，新入队的子节点属于下一层。
        int levelSize = static_cast<int>(q.size());
        std::vector<int> level;
        for (int i = 0; i < levelSize; ++i) {
            TreeNode* node = q.front();
            q.pop();
            level.push_back(node->val);
            // 按从左到右的顺序把子节点入队。
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        result.push_back(level);
    }
    return result;
}

/**
 * 3. 二叉树最大深度
 * 难度：简单
 * 思路：当前深度等于 1 + 左右子树最大深度。
 * 复杂度：时间 O(N)，空间 O(H)
 */
int maxDepth(TreeNode* root)
{
    if (!root) return 0;
    // 递归计算左右子树深度。
    int leftDepth = maxDepth(root->left);
    int rightDepth = maxDepth(root->right);
    // 当前节点本身贡献一层深度。
    return 1 + (leftDepth > rightDepth ? leftDepth : rightDepth);
}

/**
 * 4. 验证二叉搜索树
 * 难度：中等
 * 思路：每个节点都必须落在一个开区间 (low, high) 内。
 * 复杂度：时间 O(N)，空间 O(H)
 */
bool isValidBST(TreeNode* root, long long low = LLONG_MIN, long long high = LLONG_MAX)
{
    if (!root) return true;
    // 二叉搜索树要求每个节点严格位于允许范围内。
    if (root->val <= low || root->val >= high) return false;
    // 左子树所有值都要更小，右子树所有值都要更大。
    return isValidBST(root->left, low, root->val) &&
           isValidBST(root->right, root->val, high);
}

/**
 * 5. 二叉搜索树最近公共祖先
 * 难度：简单
 * 思路：在 BST 中，如果两个目标都在同一侧，就继续向该侧查找。
 * 复杂度：时间 O(H)，空间 O(1)
 */
TreeNode* lowestCommonAncestorBST(TreeNode* root, TreeNode* p, TreeNode* q)
{
    while (root) {
        if (p->val < root->val && q->val < root->val) {
            // 两个节点都更小，公共祖先一定在左子树。
            root = root->left;
        } else if (p->val > root->val && q->val > root->val) {
            // 两个节点都更大，公共祖先一定在右子树。
            root = root->right;
        } else {
            // 分叉点：两个节点分居两侧，或当前节点就是其中一个目标。
            return root;
        }
    }
    return nullptr;
}

}
