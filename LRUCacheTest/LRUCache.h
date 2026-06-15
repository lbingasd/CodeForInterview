#include <unordered_map>
using namespace std;

class LRUCache {
private:
    struct Node {
        int key;
        int value;
        Node* prev;
        Node* next;

        Node(int k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
    };

    int capacity;
    unordered_map<int, Node*> cache;

    // 虚拟头尾节点，简化插入和删除逻辑
    Node* head;
    Node* tail;

    // 从链表中移除一个节点
    void removeNode(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    // 把节点插入到头部，表示最近使用
    void addToHead(Node* node) {
        node->next = head->next;
        node->prev = head;

        head->next->prev = node;
        head->next = node;
    }

    // 把节点移动到头部
    void moveToHead(Node* node) {
        removeNode(node);
        addToHead(node);
    }

    // 删除尾部节点，尾部表示最久未使用
    Node* removeTail() {
        Node* node = tail->prev;
        removeNode(node);
        return node;
    }

public:
    LRUCache(int capacity) : capacity(capacity) {
        head = new Node(0, 0);
        tail = new Node(0, 0);

        head->next = tail;
        tail->prev = head;
    }

    int get(int key) {
        // key 不存在，返回 -1
        if (!cache.count(key)) {
            return -1;
        }

        // key 被访问，移动到头部
        Node* node = cache[key];
        moveToHead(node);

        return node->value;
    }

    void put(int key, int value) {
        // key 已存在：更新 value，并移动到头部
        if (cache.count(key)) {
            Node* node = cache[key];
            node->value = value;
            moveToHead(node);
            return;
        }

        // key 不存在：创建新节点，插入头部
        Node* node = new Node(key, value);
        cache[key] = node;
        addToHead(node);

        // 超过容量，删除最久未使用节点
        if ((int)cache.size() > capacity) {
            Node* old = removeTail();
            cache.erase(old->key);
            delete old;
        }
    }

    ~LRUCache() {
        Node* cur = head;
        while (cur) {
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
    }
};