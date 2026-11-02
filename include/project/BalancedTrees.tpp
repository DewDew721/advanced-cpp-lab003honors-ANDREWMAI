namespace project {

// ========================= AVLTree =========================

template <typename Key, typename Value, typename Compare>
int AVLTree<Key, Value, Compare>::nodeHeight(const Node* n) {
    // TODO: return the height of n, treating a null child consistently.
    return n ? n->height : 0;
}

template <typename Key, typename Value, typename Compare>
int AVLTree<Key, Value, Compare>::balanceFactor(const Node* n) {
    // TODO: return height(left) - height(right).
    return nodeHeight(n->left) - nodeHeight(n->right);
}

template <typename Key, typename Value, typename Compare>
void AVLTree<Key, Value, Compare>::destroy(Node* n) {
    // TODO: recursively destroy all nodes.
    if (n) {
        destroy(n->left);
        destroy(n->right);
        delete n;
    }
}

template <typename Key, typename Value, typename Compare>
AVLTree<Key, Value, Compare>::~AVLTree() {
    destroy(root_);
}

template <typename Key, typename Value, typename Compare>
AVLTree<Key, Value, Compare>::AVLTree(AVLTree&& other) noexcept
    : root_(other.root_), size_(other.size_), comp_(std::move(other.comp_)) {
    // TODO: leave other in a valid empty state.
    other.root_ = nullptr;
    other.size_ = 0;
}

template <typename Key, typename Value, typename Compare>
AVLTree<Key, Value, Compare>&
AVLTree<Key, Value, Compare>::operator=(AVLTree&& other) noexcept {
    // TODO: release current nodes, transfer ownership, and empty other.
    if (this != &other) {
        destroy(root_);
        root_ = other.root_;
        size_ = other.size_;
        comp_ = std::move(other.comp_);
        other.root_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}

template <typename Key, typename Value, typename Compare>
bool AVLTree<Key, Value, Compare>::empty() const noexcept {
    return size_ == 0;
}

template <typename Key, typename Value, typename Compare>
std::size_t AVLTree<Key, Value, Compare>::size() const noexcept {
    return size_;
}

template <typename Key, typename Value, typename Compare>
int AVLTree<Key, Value, Compare>::height() const noexcept {
    return nodeHeight(root_);
}

template <typename Key, typename Value, typename Compare>
typename AVLTree<Key, Value, Compare>::Node*
AVLTree<Key, Value, Compare>::rotateLeft(Node* x) {
    // TODO: perform a left rotation and update heights.
    Node* y = x->right;
    x->right = y->left;
    y->left = x;

    x->height = std::max(nodeHeight(x->left), nodeHeight(x->right)) + 1;
    y->height = std::max(nodeHeight(y->left), nodeHeight(y->right)) + 1;

    return y;
}

template <typename Key, typename Value, typename Compare>
typename AVLTree<Key, Value, Compare>::Node*
AVLTree<Key, Value, Compare>::rotateRight(Node* y) {
    // TODO: perform a right rotation and update heights.
    Node* x = y->left;
    y->left = x->right;
    x->right = y;

    y->height = std::max(nodeHeight(y->left), nodeHeight(y->right)) + 1;
    x->height = std::max(nodeHeight(x->left), nodeHeight(x->right)) + 1;

    return x;
}

template <typename Key, typename Value, typename Compare>
typename AVLTree<Key, Value, Compare>::Node*
AVLTree<Key, Value, Compare>::rebalance(Node* n) {
    // TODO: update height and handle LL, RR, LR, and RL cases.
    n->height = std::max(nodeHeight(n->left), nodeHeight(n->right)) + 1;
    int balance = balanceFactor(n);
    if (balance > 1) {
        if (balanceFactor(n->left) < 0) {
            n->left = rotateLeft(n->left);
        }
        n = rotateRight(n);
    } else if (balance < -1) {
        if (balanceFactor(n->right) > 0) {
            n->right = rotateRight(n->right);
        }
        n = rotateLeft(n);
    }
    return n;
}

template <typename Key, typename Value, typename Compare>
typename AVLTree<Key, Value, Compare>::Node*
AVLTree<Key, Value, Compare>::insert(Node* n, const Key& key,
                                     const Value& value, bool& inserted) {
    // TODO: standard BST insertion followed by AVL rebalancing.
    if (!n) {
        inserted = true;
        return new Node(key, value);
    }
    if (Compare()(key, n->key)) {
        n->left = insert(n->left, key, value, inserted);
    } else if (Compare()(n->key, key)) {
        n->right = insert(n->right, key, value, inserted);
    } else {
        n->value = value;
    }
    n = rebalance(n);
    return n;
}

template <typename Key, typename Value, typename Compare>
void AVLTree<Key, Value, Compare>::insert(const Key& key, const Value& value) {
    bool inserted = false;
    root_ = insert(root_, key, value, inserted);
    if (inserted) {
        ++size_;
    }
}

template <typename Key, typename Value, typename Compare>
typename AVLTree<Key, Value, Compare>::Node*
AVLTree<Key, Value, Compare>::minimum(Node* n) {
    // TODO: return the leftmost node.
    while (n && n->left) {
        n = n->left;
    }
    return n;
}

template <typename Key, typename Value, typename Compare>
typename AVLTree<Key, Value, Compare>::Node*
AVLTree<Key, Value, Compare>::erase(Node* n, const Key& key, bool& removed) {
    // TODO: standard BST deletion followed by AVL rebalancing.
    if (!n) {
        return nullptr;
    }

    if (Compare()(key, n->key)) {
        n->left = erase(n->left, key, removed);
    } else if (Compare()(n->key, key)) {
        n->right = erase(n->right, key, removed);
    } else {
        removed = true;
        if (!n->left || !n->right) {
            Node* temp = n->left ? n->left : n->right;
            delete n;
            return temp;
        } else {
            Node* temp = minimum(n->right);
            n->key = temp->key;
            n->value = temp->value;
            n->right = erase(n->right, temp->key, removed);
        }
    }

    return rebalance(n);
}

template <typename Key, typename Value, typename Compare>
bool AVLTree<Key, Value, Compare>::erase(const Key& key) {
    bool removed = false;
    root_ = erase(root_, key, removed);
    if (removed) {
        --size_;
    }
    return removed;
}

template <typename Key, typename Value, typename Compare>
const typename AVLTree<Key, Value, Compare>::Node*
AVLTree<Key, Value, Compare>::findNode(const Node* n, const Key& key) const {
    // TODO: perform ordered search.
    while (n) {
        if (Compare()(key, n->key)) {
            n = n->left;
        } else if (Compare()(n->key, key)) {
            n = n->right;
        } else {
            return n; // Key found
        }
    }
    return nullptr;
}

template <typename Key, typename Value, typename Compare>
bool AVLTree<Key, Value, Compare>::contains(const Key& key) const {
    return findNode(root_, key) != nullptr;
}

template <typename Key, typename Value, typename Compare>
const Value* AVLTree<Key, Value, Compare>::find(const Key& key) const {
    const Node* n = findNode(root_, key);
    return n ? &n->value : nullptr;
}

template <typename Key, typename Value, typename Compare>
void AVLTree<Key, Value, Compare>::inorder(const Node* n,
                                           std::vector<Key>& out) {
    // TODO: append keys in sorted order.
    if (n) {
        inorder(n->left, out);
        out.push_back(n->key);
        inorder(n->right, out);
    }
}

template <typename Key, typename Value, typename Compare>
std::vector<Key> AVLTree<Key, Value, Compare>::inorder() const {
    std::vector<Key> out;
    inorder(root_, out);
    return out;
}

template <typename Key, typename Value, typename Compare>
bool AVLTree<Key, Value, Compare>::validate(const Node* n, const Key* lo,
                                            const Key* hi, int& computedHeight) {
    // TODO: verify BST ordering, stored heights, and AVL balance.
    if (!n) {
        computedHeight = 0;
        return true;
    }

    if ((lo && Compare()(n->key, *lo)) || (hi && Compare()(*hi, n->key))) {
        return false; // BST property violated
    }

    int leftHeight = 0;
    int rightHeight = 0;
    if (!validate(n->left, lo, &n->key, leftHeight) ||
        !validate(n->right, &n->key, hi, rightHeight)) {
        return false;
    }

    computedHeight = std::max(leftHeight, rightHeight) + 1;
    return n->height == computedHeight &&
           leftHeight - rightHeight >= -1 && leftHeight - rightHeight <= 1;
}

template <typename Key, typename Value, typename Compare>
bool AVLTree<Key, Value, Compare>::isValidAVL() const {
    int h = 0;
    return validate(root_, nullptr, nullptr, h);
}

// ========================= SplayTree =========================

template <typename Key, typename Value, typename Compare>
void SplayTree<Key, Value, Compare>::destroy(Node* n) {
    // TODO: recursively destroy nodes.
    if (n) {
        destroy(n->left);
        destroy(n->right);
        delete n;
    }
}

template <typename Key, typename Value, typename Compare>
SplayTree<Key, Value, Compare>::~SplayTree() {
    destroy(root_);
}

template <typename Key, typename Value, typename Compare>
SplayTree<Key, Value, Compare>::SplayTree(SplayTree&& other) noexcept
    : root_(other.root_), size_(other.size_), comp_(std::move(other.comp_)) {
    // TODO: leave other empty.
    other.root_ = nullptr;
    other.size_ = 0;
}

template <typename Key, typename Value, typename Compare>
SplayTree<Key, Value, Compare>&
SplayTree<Key, Value, Compare>::operator=(SplayTree&& other) noexcept {
    // TODO: release current nodes, transfer ownership, and empty other.
    if (this != &other) {
        destroy(root_);
        root_ = other.root_;
        size_ = other.size_;
        comp_ = std::move(other.comp_);
        other.root_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}

template <typename Key, typename Value, typename Compare>
bool SplayTree<Key, Value, Compare>::empty() const noexcept {
    return size_ == 0;
}

template <typename Key, typename Value, typename Compare>
std::size_t SplayTree<Key, Value, Compare>::size() const noexcept {
    return size_;
}

template <typename Key, typename Value, typename Compare>
void SplayTree<Key, Value, Compare>::rotateLeft(Node* x) {
    // TODO: implement a parent-aware left rotation.
    Node* y = x->right;
    x->right = y->left;
    if (y->left) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (!x->parent) {
        root_ = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

template <typename Key, typename Value, typename Compare>
void SplayTree<Key, Value, Compare>::rotateRight(Node* x) {
    // TODO: implement a parent-aware right rotation.
    Node* y = x->left;
    x->left = y->right;
    if (y->right) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (!x->parent) {
        root_ = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

template <typename Key, typename Value, typename Compare>
void SplayTree<Key, Value, Compare>::splay(Node* x) {
    // TODO: implement zig, zig-zig, and zig-zag cases until x is root.
    if (!x) return;
    while (x->parent) {
        if (!x->parent->parent) {
            // Zig case
            if (x == x->parent->left) {
                rotateRight(x->parent);
            } else {
                rotateLeft(x->parent);
            }
        } else if (x == x->parent->left && x->parent == x->parent->parent->left) {
            // Zig-zig case
            rotateRight(x->parent->parent);
            rotateRight(x->parent);
        } else if (x == x->parent->right && x->parent == x->parent->parent->right) {
            // Zig-zig case
            rotateLeft(x->parent->parent);
            rotateLeft(x->parent);
        } else if (x == x->parent->right && x->parent == x->parent->parent->left) {
            // Zig-zag case
            rotateLeft(x->parent);
            rotateRight(x->parent);
        } else {
            // Zig-zag case
            rotateRight(x->parent);
            rotateLeft(x->parent);
        }
    }
}

template <typename Key, typename Value, typename Compare>
typename SplayTree<Key, Value, Compare>::Node*
SplayTree<Key, Value, Compare>::findNode(const Key& key) {
    // TODO: search for key, remembering the last accessed node,
    // then splay the found node or last accessed node.
    if (!root_) return nullptr;
    Node* current = root_;
    Node* lastAccessed = nullptr;

    while (current) {
        lastAccessed = current;
        if (Compare()(key, current->key)) {
            current = current->left;
        } else if (Compare()(current->key, key)) {
            current = current->right;
        } else {
            splay(current);
            return current; // Key found
        }
    }
    splay(lastAccessed);
    return nullptr;
}

template <typename Key, typename Value, typename Compare>
bool SplayTree<Key, Value, Compare>::contains(const Key& key) {
    return findNode(key) != nullptr;
}

template <typename Key, typename Value, typename Compare>
const Value* SplayTree<Key, Value, Compare>::find(const Key& key) {
    Node* n = findNode(key);
    return n ? &n->value : nullptr;
}

template <typename Key, typename Value, typename Compare>
void SplayTree<Key, Value, Compare>::insert(const Key& key,
                                            const Value& value) {
    // TODO: insert as a BST node and splay the inserted/found node.
    if (!root_) {
        root_ = new Node(key, value);
        ++size_;
        return;
    }
    Node* current = root_;
    Node* parent = nullptr;

    while (current) {
        parent = current;
        if (Compare()(key, current->key)) {
            current = current->left;
        } else if (Compare()(current->key, key)) {
            current = current->right;
        } else {
            // Key already exists, update value and splay
            current->value = value;
            splay(current);
            return;
        }
    }
    // Key does not exist, insert it
    Node* newNode = new Node(key, value);
    newNode->parent = parent;
    if (parent) {
        if (Compare()(key, parent->key)) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }
    } else {
        root_ = newNode;
    }
    ++size_;
    splay(newNode);
}

template <typename Key, typename Value, typename Compare>
void SplayTree<Key, Value, Compare>::transplant(Node* u, Node* v) {
    // TODO: replace subtree u with v while maintaining parent pointers.
    if (!u->parent) {
        root_ = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v) {
        v->parent = u->parent;
    }
}

template <typename Key, typename Value, typename Compare>
typename SplayTree<Key, Value, Compare>::Node*
SplayTree<Key, Value, Compare>::subtreeMinimum(Node* n) {
    // TODO: return leftmost node.
    while (n && n->left) {
        n = n->left;
    }
    return n;
}

template <typename Key, typename Value, typename Compare>
bool SplayTree<Key, Value, Compare>::erase(const Key& key) {
    // TODO: splay the target to the root, remove it, and join the
    // left/right subtrees correctly.
    Node* target = findNode(key);
    if (!target) {
        return false; // Key not found
    }
    if (!target->left) {
        transplant(target, target->right);
    } else if (!target->right) {
        transplant(target, target->left);
    } else {
        Node* minRight = subtreeMinimum(target->right);
        if (minRight->parent != target) {
            transplant(minRight, minRight->right);
            minRight->right = target->right;
            minRight->right->parent = minRight;
        }
        transplant(target, minRight);
        minRight->left = target->left;
        minRight->left->parent = minRight;
    }
    delete target;
    --size_;
    return true;
}

template <typename Key, typename Value, typename Compare>
const Key* SplayTree<Key, Value, Compare>::rootKey() const noexcept {
    return root_ ? &root_->key : nullptr;
}

template <typename Key, typename Value, typename Compare>
void SplayTree<Key, Value, Compare>::inorder(const Node* n,
                                             std::vector<Key>& out) {
    // TODO: append keys in sorted order.
    if (n) {
        inorder(n->left, out);
        out.push_back(n->key);
        inorder(n->right, out);
    }
}

template <typename Key, typename Value, typename Compare>
std::vector<Key> SplayTree<Key, Value, Compare>::inorder() const {
    std::vector<Key> out;
    inorder(root_, out);
    return out;
}

template <typename Key, typename Value, typename Compare>
std::size_t SplayTree<Key, Value, Compare>::depth(const Node* n) {
    // TODO: compute subtree height/depth.
    if (!n) {
        return 0;
    }
    std::size_t leftDepth = depth(n->left);
    std::size_t rightDepth = depth(n->right);
    return (leftDepth > rightDepth ? leftDepth : rightDepth) + 1;
}

template <typename Key, typename Value, typename Compare>
std::size_t SplayTree<Key, Value, Compare>::depth() const noexcept {
    return depth(root_);
}

} // namespace project
