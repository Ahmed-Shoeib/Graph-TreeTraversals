//C++ code
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// TreeNode structure
template <typename T>
struct TreeNode {
    T data;
    TreeNode* left;
    TreeNode* right;

    TreeNode(T val) : data(val), left(nullptr), right(nullptr) {}
};

// Custom Queue class
template <typename T>
class Queue {
public:
    struct Node {
        T data;
        Node* next;
        Node(T val) : data(val), next(nullptr) {}
    };

    Queue() : front(nullptr), rear(nullptr) {}

    void enqueue(T value) {
        Node* newNode = new Node(value);
        if (isEmpty()) {
            front = rear = newNode;
        }
        else {
            rear->next = newNode;
            rear = newNode;
        }
    }

    T dequeue() {
        if (isEmpty()) {
            cout << "Queue is empty!" << endl;
            return T();
        }
        Node* temp = front;
        T value = front->data;
        front = front->next;
        if (front == nullptr) rear = nullptr;
        delete temp;
        return value;
    }

    bool isEmpty() {
        return front == nullptr;
    }

private:
    Node* front;
    Node* rear;
};

// Function to create a tree from user input
template <typename T>
TreeNode<T>* buildTreeFromInput() {
    cout << "Enter the root node value: ";
    T value;
    cin >> value;

    if (value == -1) return nullptr;

    TreeNode<T>* root = new TreeNode<T>(value);
    Queue<TreeNode<T>*> q;
    q.enqueue(root);

    while (!q.isEmpty()) {
        TreeNode<T>* current = q.dequeue();

        // Ask for left child
        cout << "Enter left child of " << current->data << " (-1 for no child): ";
        cin >> value;
        if (value != -1) {
            current->left = new TreeNode<T>(value);
            q.enqueue(current->left);
        }

        // Ask for right child
        cout << "Enter right child of " << current->data << " (-1 for no child): ";
        cin >> value;
        if (value != -1) {
            current->right = new TreeNode<T>(value);
            q.enqueue(current->right);
        }
    }

    return root;
}

// Traversals
template <typename T>
void preorder(TreeNode<T>* root, ofstream& file) {
    if (!root) return;
    cout << root->data << " ";
    file << root->data << " ";
    preorder(root->left, file);
    preorder(root->right, file);
}

template <typename T>
void inorder(TreeNode<T>* root, ofstream& file) {
    if (!root) return;
    inorder(root->left, file);
    cout << root->data << " ";
    file << root->data << " ";
    inorder(root->right, file);
}

template <typename T>
void postorder(TreeNode<T>* root, ofstream& file) {
    if (!root) return;
    postorder(root->left, file);
    postorder(root->right, file);
    file << root->data << " ";
    cout << root->data << " ";
}

// Level Order Traversal
template <typename T>
void levelOrder(TreeNode<T>* root) {
    if (!root) return;
    Queue<TreeNode<T>*> q;
    q.enqueue(root);

    while (!q.isEmpty()) {
        TreeNode<T>* current = q.dequeue();
        cout << current->data << " ";
        if (current->left) q.enqueue(current->left);
        if (current->right) q.enqueue(current->right);
    }
    cout << endl;
}

// Compute Height
template <typename T>
int treeHeight(TreeNode<T>* root) {
    if (!root) return 0;
    return 1 + max(treeHeight(root->left), treeHeight(root->right));
}

// Compute Sum of Nodes
template <typename T>
T sumOfNodes(TreeNode<T>* root) {
    if (!root) return 0;
    return root->data + sumOfNodes(root->left) + sumOfNodes(root->right);
}

// Save Traversals to File
template <typename T>
void saveTreeToFiles(TreeNode<T>* root, bool& treeSaved) {
    if (treeSaved) {
        return; // Do nothing if the tree is already saved
    }

    // Open files in write mode to overwrite the existing data
    ofstream preorderFile("preorder.txt", ios::app);  // Overwrite file
    ofstream inorderFile("inorder.txt", ios::app);    // Overwrite file
    ofstream postorderFile("postorder.txt", ios::app); // Overwrite file
    ofstream originalTreeFile("original_tree.txt", ios::app);  // To save original tree

    if (!preorderFile || !inorderFile || !postorderFile || !originalTreeFile) {
        cout << "Error opening files for traversal output!" << endl;
        return;
    }

    // Add a separator for new tree data
    originalTreeFile << "\n----- NEW TREE -----\n";
    preorderFile << "\n----- NEW TREE -----\n";
    inorderFile << "\n----- NEW TREE -----\n";
    postorderFile << "\n----- NEW TREE -----\n";


    originalTreeFile << endl; // End line after original tree

    // Mark that the tree has been saved
    treeSaved = true;
}



// Display Menu
void displayMenu() {
    cout << "\nMenu:\n";
    cout << "1. Insert New tree\n";
    cout << "2. Preorder Traversal\n";
    cout << "3. Inorder Traversal\n";
    cout << "4. Postorder Traversal\n";
    cout << "5. Level Traversal (BFS)\n";
    cout << "6. Print Tree Height\n";
    cout << "7. Compute Sum of Nodes\n";
    cout << "8. Build Expression Tree\n";
    cout << "9. Evaluate Expression Tree\n";
    cout << "10. Exit\n";
}

TreeNode<string>* buildExpressionTree() {
    cout << "Enter the root node value (operator or operand): ";
    string value;
    cin >> value;

    if (value == "#") return nullptr; // '#' denotes no node

    TreeNode<string>* root = new TreeNode<string>(value);
    Queue<TreeNode<string>*> q;
    q.enqueue(root);

    while (!q.isEmpty()) {
        TreeNode<string>* current = q.dequeue();

        // Ask for left child
        cout << "Enter left child of " << current->data << " (operator/operand or '#' for no child): ";
        cin >> value;
        if (value != "#") {
            current->left = new TreeNode<string>(value);
            q.enqueue(current->left);
        }

        // Ask for right child
        cout << "Enter right child of " << current->data << " (operator/operand or '#' for no child): ";
        cin >> value;
        if (value != "#") {
            current->right = new TreeNode<string>(value);
            q.enqueue(current->right);
        }
    }

    return root;
}

template <typename T>
void preorderExpression(TreeNode<T>* root) {
    if (!root) return;
    cout << root->data << " ";
    preorderExpression(root->left);
    preorderExpression(root->right);
}

template <typename T>
void inorderExpression(TreeNode<T>* root) {
    if (!root) return;
    inorderExpression(root->left);
    cout << root->data << " ";
    inorderExpression(root->right);
}

template <typename T>
void postorderExpression(TreeNode<T>* root) {
    if (!root) return;
    postorderExpression(root->left);
    postorderExpression(root->right);
    cout << root->data << " ";
}

int main() {
    TreeNode<int>* root = nullptr;
    TreeNode<string>* exprRoot = nullptr;
    int choice;
    bool treeSaved = false;  // Flag to track if the tree has been saved

    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            root = buildTreeFromInput<int>();
            treeSaved = false;  // Reset the treeSaved flag when a new tree is built
            break;
        case 2:
            saveTreeToFiles(root, treeSaved);  // Save tree if not saved yet
            {
                ofstream preorderFile("preorder.txt", ios::app);  // Append traversal data
                preorder(root, preorderFile);
                preorderFile << endl;
                cout << "\nPreorder traversal appended to file." << endl;
            }
            break;
        case 3:
            saveTreeToFiles(root, treeSaved);  // Save tree if not saved yet
            {
                ofstream inorderFile("inorder.txt", ios::app);  // Append traversal data
                inorder(root, inorderFile);
                inorderFile << endl;
                cout << "\nInorder traversal appended to file." << endl;
            }
            break;
        case 4:
            saveTreeToFiles(root, treeSaved);  // Save tree if not saved yet
            {
                ofstream postorderFile("postorder.txt", ios::app);  // Append traversal data
                postorder(root, postorderFile);
                postorderFile << endl;
                cout << "\nPostorder traversal appended to file." << endl;
            }
            break;
        case 5:
            cout << "Level Order Traversal: ";
            levelOrder(root);
            break;
        case 6:
            cout << "Tree Height: " << treeHeight(root) << endl;
            break;
        case 7:
            cout << "Sum of Nodes: " << sumOfNodes(root) << endl;
            break;
        case 8:
            exprRoot = buildExpressionTree();
            cout << "Expression tree has been created." << endl;
            break;
        case 9:
            if (exprRoot) {
                cout << "\nChoose order to print the equation:\n";
                cout << "1. Preorder\n";
                cout << "2. Inorder\n";
                cout << "3. Postorder\n";
                cout << "Enter your choice: ";
                int orderChoice;
                cin >> orderChoice;

                cout << "Expression Equation: ";
                switch (orderChoice) {
                case 1: // Preorder
                    preorderExpression(exprRoot);
                    break;
                case 2: // Inorder
                    inorderExpression(exprRoot);
                    break;
                case 3: // Postorder
                    postorderExpression(exprRoot);
                    break;
                default:
                    cout << "\nInvalid choice. Returning to menu.";
                }
                cout << endl; // End line after printing the equation
            }
            else {
                cout << "Expression tree is not built yet." << endl;
            }
            break;

        case 10:
            cout << "Exiting program. Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 10);


}