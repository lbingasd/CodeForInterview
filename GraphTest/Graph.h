#pragma once

#include <queue>
#include <string>
#include <utility>
#include <vector>

namespace GraphTest {

/**
 * 1. 岛屿数量
 * 难度：中等
 * 思路：DFS/BFS 染色。每发现一块新陆地，就把整座岛标记掉。
 * 复杂度：时间 O(M * N)，最坏空间 O(M * N)
 */
void dfsIsland(std::vector<std::string>& grid, int row, int col)
{
    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());
    // 越界、遇到水、或遇到已访问格子时停止。
    if (row < 0 || row >= rows || col < 0 || col >= cols || grid[row][col] != '1') {
        return;
    }

    // 将陆地改成水，表示该格子已经访问过。
    grid[row][col] = '0';
    // 继续搜索上下左右四个连通方向。
    dfsIsland(grid, row + 1, col);
    dfsIsland(grid, row - 1, col);
    dfsIsland(grid, row, col + 1);
    dfsIsland(grid, row, col - 1);
}

int numIslands(std::vector<std::string> grid)
{
    if (grid.empty() || grid[0].empty()) return 0;

    int count = 0;
    for (int row = 0; row < static_cast<int>(grid.size()); ++row) {
        for (int col = 0; col < static_cast<int>(grid[0].size()); ++col) {
            if (grid[row][col] == '1') {
                // 新的未访问陆地表示发现了一座新岛。
                ++count;
                // 把整座连通岛屿标记掉，避免重复计数。
                dfsIsland(grid, row, col);
            }
        }
    }
    return count;
}

/**
 * 2. 课程表
 * 难度：中等
 * 思路：拓扑排序。如果所有节点都能通过入度为 0 的方式移除，则无环。
 * 复杂度：时间 O(V + E)，空间 O(V + E)
 */
bool canFinish(int numCourses, const std::vector<std::vector<int>>& prerequisites)
{
    // 邻接表保存每门课解锁后的后续课程。
    std::vector<std::vector<int>> graph(numCourses);
    // 入度数组统计每门课还剩多少前置课程。
    std::vector<int> indegree(numCourses, 0);

    for (const auto& edge : prerequisites) {
        int course = edge[0];
        int pre = edge[1];
        graph[pre].push_back(course);
        ++indegree[course];
    }

    std::queue<int> q;
    for (int i = 0; i < numCourses; ++i) {
        // 没有前置课程的课可以最先学习。
        if (indegree[i] == 0) q.push(i);
    }

    int visited = 0;
    while (!q.empty()) {
        int pre = q.front();
        q.pop();
        // 学完这门课，相当于把它从剩余图中移除。
        ++visited;
        for (int course : graph[pre]) {
            // 依赖课程少了一个前置条件。
            if (--indegree[course] == 0) {
                // 所有前置条件都满足后，该课程可以入队。
                q.push(course);
            }
        }
    }
    // 如果存在环，会有节点永远无法变成入度 0。
    return visited == numCourses;
}

/**
 * 3. 无权图最短路径
 * 难度：中等
 * 思路：BFS 第一次访问到某个节点时，一定是最短距离。
 * 复杂度：时间 O(V + E)，空间 O(V)
 */
int shortestPathBFS(const std::vector<std::vector<int>>& graph, int start, int target)
{
    if (start == target) return 0;

    // 距离为 -1 表示该节点尚未访问。
    std::vector<int> dist(graph.size(), -1);
    std::queue<int> q;
    dist[start] = 0;
    q.push(start);

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        for (int next : graph[node]) {
            // 广度优先搜索第一次到达某个节点时，就是它的最短距离。
            if (dist[next] != -1) continue;
            dist[next] = dist[node] + 1;
            if (next == target) return dist[next];
            q.push(next);
        }
    }
    return -1;
}

/**
 * 4. 图像填充
 * 难度：简单
 * 思路：BFS/DFS 将所有与起点连通且颜色相同的格子改色。
 * 复杂度：时间 O(M * N)，空间 O(M * N)
 */
std::vector<std::vector<int>> floodFill(
    std::vector<std::vector<int>> image,
    int sr,
    int sc,
    int newColor)
{
    // 所有与起点连通且颜色为 oldColor 的格子都要改成 newColor。
    int oldColor = image[sr][sc];
    if (oldColor == newColor) return image;

    int rows = static_cast<int>(image.size());
    int cols = static_cast<int>(image[0].size());
    std::queue<std::pair<int, int>> q;
    q.push({sr, sc});
    // 入队时就改色，避免同一个格子被重复入队。
    image[sr][sc] = newColor;

    const int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    while (!q.empty()) {
        auto cell = q.front();
        q.pop();
        for (const auto& dir : dirs) {
            int nr = cell.first + dir[0];
            int nc = cell.second + dir[1];
            // 跳过越界格子，以及不属于原始区域的格子。
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols || image[nr][nc] != oldColor) {
                continue;
            }
            // 标记并入队合法邻居。
            image[nr][nc] = newColor;
            q.push({nr, nc});
        }
    }
    return image;
}

}
