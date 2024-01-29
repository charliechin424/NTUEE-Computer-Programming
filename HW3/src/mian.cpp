#include <iostream>
#include <vector>
using namespace std;

class TreeNode{
    friend class BST;
    public:
        TreeNode():val(0), left(NULL), right(NULL){};
    private:
        int val;
        TreeNode* left;
        TreeNode* right;
};

class BST{
    public:
        BST():root(NULL){};
        int Depth(TreeNode*);
        void preorderTraversal(TreeNode*);
        void inorderTraversal(TreeNode*);
        void postorderTraversal(TreeNode*);
        int** levelorderTraversal(TreeNode*, int, int*);
        void insert_to_leaf(int);
        void maxKey();
        void minKey();
        int successor(int);
        int predecessor(int);
        void delNode(int);
        TreeNode* getTree();
    private:
        void levelorder(TreeNode*, int**, int , int*);
        void BSTinsert(TreeNode*, int, bool*);  //utility function: 用來實現insert_to_leaf 的 recursive function
        void flatten(TreeNode*, vector <int> &,int*, int); ////utility function: 把所有 node 依照小到大存進一個array中
        void deleteNode(TreeNode*, int, bool*); //utility function: 用來實現delNode 的 recursive function
        TreeNode* root;
};

TreeNode* BST::getTree(){
    return root;
}

int BST::Depth(TreeNode* root){
    if (root == NULL){
        return 0;
    }
    int right_height = Depth (root -> right);
    int left_height = Depth (root -> left);
    if (right_height >= left_height){
        return right_height + 1;
    } else {
        return left_height + 1;
    }
}

void BST::preorderTraversal(TreeNode* root){
    TreeNode* trace = root;
    if (trace == NULL){
        return;
    }
    cout << trace -> val << " " ;
    preorderTraversal(trace -> left);
    preorderTraversal(trace -> right);
}

void BST::inorderTraversal(TreeNode* root){
    TreeNode* trace = root;
    if (trace == NULL){
        return;
    }
    inorderTraversal(trace -> left);
    cout << trace -> val << " ";
    inorderTraversal(trace -> right);
}

void BST::postorderTraversal(TreeNode* root){
    TreeNode* trace = root;
    if (trace == NULL){
        return;
    }
    postorderTraversal(trace -> left);
    postorderTraversal(trace -> right);
    cout << trace -> val << " " ;
}

void BST::levelorder(TreeNode* root, int** ans, int depth, int* time){
    if (root == NULL){
        return;
    }
    depth++;
    ans[depth][time[depth]] = root -> val;
    time[depth]++;
    levelorder(root -> left, ans, depth, time);
    levelorder(root -> right, ans, depth, time);
}

int** BST::levelorderTraversal(TreeNode* root, int depth, int* returnColSizes) {
    int** ans = new int* [depth];
    for (int i=0 ; i<depth ; ++i){
        ans[i] = new int [1000];
    }
    int time[depth] = {0};
    levelorder(root, ans, -1, time);
    for (int i=0 ; i<depth ; ++i){
        returnColSizes[i] = time[i];
    }
    return ans;
}

void BST::BSTinsert(TreeNode* Root, int key, bool* error){
    if (Root -> val == key){
        cout << "the node is in the BST" << endl;
        *error = true;
        return;
    }
    if (Root -> left == NULL && Root -> val > key){
        Root -> left = new TreeNode [1];
        Root -> left -> val = key;
        return;
    }
    if (Root -> right == NULL && Root -> val < key){
        Root -> right = new TreeNode [1];
        Root -> right -> val = key;
        return;
    }
    if (Root -> val < key){
        return BSTinsert(Root -> right, key, error);
    } else {
        return BSTinsert(Root -> left, key, error);
    }
}

void BST::insert_to_leaf(int key){
    if (root == NULL){
        root = new TreeNode [1];
        root -> val = key;
        cout << "插入完成" << endl;
        return;
    }
    bool error = false;
    BSTinsert(root, key, &error);
    if (error == false){
        cout << "插入完成" << endl;
    }
}

void BST::maxKey(){
    TreeNode* trace = root;
    while(trace -> right != NULL){
        trace = trace -> right;
    }
    cout << trace -> val << endl;
}

void BST::minKey(){
    TreeNode* trace = root;
    while(trace -> left != NULL){
        trace = trace -> left;
    }
    cout << trace -> val << endl;
}

void BST::flatten(TreeNode* root, vector <int> &num,int* index, int key){
    if (root == NULL){
        return;
    }
    flatten(root -> left, num, index, key);
    if (root -> val == key){
        *index = num.size();
    }
    num.push_back(root -> val);
    flatten(root -> right, num, index, key);
}

int BST::successor(int key){
    vector <int> num(0);
    int index = -1000000;
    flatten(root, num, &index, key);
    if (index == -1000000){
        cout << key << " is not in the BST" << endl;
        return -1000000;
    }
    if (index == num.size()-1){
        cout << key << " is the Max of the Tree, the successor of " << key << " doesn't exist" << endl;
        return -1000000;
    }
    return num[index+1];
}

int BST::predecessor(int key){
    vector <int> num(0);
    int index = -1000000;
    flatten(root, num, &index, key);
    if (index == -1000000){
        cout << key << " is not in the BST" << endl;
        return -1000000;
    }
    if (index == 0){
        cout << key << " is the Min of the Tree, the predecessor of " << key << " doesn't exist" << endl;
        return -1000000;
    }
    return num[index-1];
}

void BST::deleteNode(TreeNode* root, int key, bool* error){
    TreeNode* ptr = root;
    if (root == NULL){
        return;
    }
    TreeNode* trace = root;
    int orie = -1;
    while(ptr != NULL && ptr -> val != key){
        trace = ptr;
        if (ptr -> val > key){
            ptr = ptr -> left;
            orie = 0;
        } else {
            ptr = ptr -> right;
            orie = 1;
        }
    }
    if (ptr == NULL){
        cout << "the node is not in the BST" << endl;
        *error = true;
        return;
    }
    if (ptr -> right == NULL && ptr -> left == NULL){
        if (orie == 1){
            trace -> right = NULL;
        } else {
            trace -> left = NULL;
        }
    } else if (ptr -> right == NULL && ptr -> left != NULL){
        if (orie == 1){
            trace -> right =  ptr -> left;
        } else {
            trace -> left =  ptr -> left;
        }
    } else if (ptr -> right != NULL && ptr -> left == NULL){
        if (orie == 1){
            trace -> right =  ptr -> right;
        } else {
            trace -> left =  ptr -> right;
        }
    } else {
        int succ = successor(key);
        deleteNode(root, succ, error);
        ptr -> val = succ;
    }
}

void BST::delNode(int key){
    if (root != NULL && root -> val == key && root -> right == NULL && root -> left == NULL){
        root = NULL;
        cout << "刪除完成" << endl;
        return;
    }
    if (root != NULL && root -> val == key && root -> right != NULL && root -> left == NULL){
        root = root -> right;
        cout << "刪除完成" << endl;
        return;
    }
    if (root != NULL && root -> val == key && root -> right == NULL && root -> left != NULL){
        root = root -> left;
        cout << "刪除完成" << endl;
        return;
    }
    bool error = false;
    deleteNode(root, key, &error);
    if (error == false){
        cout << "刪除完成" << endl;
    }
}

int main(){
    cout << "歡迎使用Binary Search Tree!" << endl;
    cout << "operation:" << endl << "1: Tree Height\n" << "2: Preorder\n"
         << "3: Inorder\n" << "4: Postorder\n" << "5: levelorder\n" << "6: Insert a node\n" << "7: Max\n" << "8: Min\n"
         << "9: Successor\n" << "10: Predecessor\n" << "11: Delete a node\n";
    BST tree;
    int instruction;
    do
    {
        cout << "請輸入要執行的指令: ";
        cin >> instruction;
        TreeNode* root = NULL;
        int key;
        int successor = 0;
        int predecessor = 0;
        switch(instruction)
        {
            case 0:
                cout << "執行結束";
                break;
            case 1:
                cout << "樹的最大高度為: " << tree.Depth(tree.getTree()) << endl;
                break;
            case 2:
                root = tree.getTree();
                if (root == NULL){
                    cout << "This is an empty tree" << endl;
                } else {
                    cout << "the preorder of the BST: ";
                    tree.preorderTraversal(tree.getTree());
                    cout << endl;
                }
                break;
            case 3:
                root = tree.getTree();
                if (root == NULL){
                    cout << "This is an empty tree" << endl;
                } else {
                    cout << "the inorder of the BST: ";
                    tree.inorderTraversal(tree.getTree());
                    cout << endl;
                }
                break;
            case 4:
                root = tree.getTree();
                if (root == NULL){
                    cout << "This is an empty tree" << endl;
                } else {
                    cout << "the postorder of the BST: ";
                    tree.postorderTraversal(tree.getTree());
                    cout << endl;
                }
                break;
            case 5:
                root = tree.getTree();
                if (root == NULL){
                    cout << "This is an empty tree" << endl;
                } else {
                    cout << "the levelorder of the BST: \n";
                    int depth = tree.Depth(root);
                    int* returnColSizes = new int [depth];
                    int** level_arr = tree.levelorderTraversal(root, depth, returnColSizes);
                    for (int i=0 ; i<depth ; ++i){
                        cout << "level " << i+1 << " : ";
                        for (int j=0 ; j<returnColSizes[i] ; ++j){
                             cout << level_arr[i][j] << " ";
                        }
                        cout << endl;
                    }
                }
                break;
            case 6:
                cout << "請輸入要插入的值: ";
                cin >> key;
                tree.insert_to_leaf(key);
                break;
            case 7:
                root = tree.getTree();
                if (root == NULL){
                    cout << "This is an empty tree" << endl;
                } else {
                    cout << "BST最大值是: ";
                    tree.maxKey();
                }
                break;
            case 8:
                root = tree.getTree();
                if (root == NULL){
                    cout << "This is an empty tree" << endl;
                } else {
                    cout << "BST最小值是: ";
                    tree.minKey();
                }
                break;
            case 9:
                root = tree.getTree();
                if (root == NULL){
                    cout << "This is an empty tree" << endl;
                } else {
                    cout << "請輸入要查詢的successor值: ";
                    cin >> key;
                    successor = tree.successor(key);
                    if (successor != -1000000){
                        cout << "the successor of the " << key << " is " << successor << endl;
                    }
                }
                break;
            case 10:
                root = tree.getTree();
                if (root == NULL){
                    cout << "This is an empty tree" << endl;
                } else {
                    cout << "請輸入要查詢的predecessor值: ";
                    cin >> key;
                    predecessor = tree.predecessor(key);
                    if (predecessor != -1000000){
                        cout << "the predecessor of the " << key << " is " << predecessor << endl;
                    }
                }
                break;
            case 11:
                root = tree.getTree();
                if (root == NULL){
                    cout << "This is an empty tree" << endl;
                } else {
                    cout << "請輸入要刪除的值: ";
                    cin >> key;
                    tree.delNode(key);
                }
                break;
        }
    } while(instruction != 0);
    return 0;
}
