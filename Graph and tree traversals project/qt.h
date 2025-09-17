#include "QtWidgetsApplication1.h"
#include <QPainter>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QSplitter>
#include <QTextEdit>
#include <QLineEdit>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QListWidget>
#include <QQueue>

// Constructor
QtWidgetsApplication1::QtWidgetsApplication1(QWidget* parent)
    : QWidget(parent), root(nullptr)
{
    // Initialize UI components
    btnInsertNode = new QPushButton("Add Node", this);
    btnPreorder = new QPushButton("Preorder", this);
    btnInorder = new QPushButton("Inorder", this);
    btnPostorder = new QPushButton("Postorder", this);
    btnLevelOrder = new QPushButton("Level Order", this);
    btnNewTree = new QPushButton("New Tree", this);
    btnSum = new QPushButton("Sum of Tree", this);
    btnHeight = new QPushButton("Tree Height", this);
    txtNodeValue = new QLineEdit(this);
    cmbDirection = new QComboBox(this);
    cmbParentNode = new QComboBox(this);  // ComboBox for parent node selection
    txtResult = new QTextEdit(this);

    cmbDirection->addItem("Left");
    cmbDirection->addItem("Right");

    // Layouts for buttons
    QVBoxLayout* buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(btnInsertNode);
    buttonLayout->addWidget(btnPreorder);
    buttonLayout->addWidget(btnInorder);
    buttonLayout->addWidget(btnPostorder);
    buttonLayout->addWidget(btnLevelOrder);
    buttonLayout->addWidget(new QLabel("Node Value:"));
    buttonLayout->addWidget(txtNodeValue);
    buttonLayout->addWidget(cmbDirection);
    buttonLayout->addWidget(new QLabel("Select Parent Node:"));
    buttonLayout->addWidget(cmbParentNode);  // Add Parent Node ComboBox
    buttonLayout->addWidget(btnNewTree);
    buttonLayout->addWidget(btnSum);
    buttonLayout->addWidget(btnHeight);
    buttonLayout->addWidget(txtResult);

    // Create the splitter
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);

    // Create a container widget for the buttons and set its layout
    QWidget* buttonWidget = new QWidget;
    buttonWidget->setLayout(buttonLayout);

    // Add the button widget and the drawing area to the splitter
    splitter->addWidget(buttonWidget);
    QWidget* drawingArea = new QWidget;
    splitter->addWidget(drawingArea);

    // Set the main layout to the splitter
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(splitter);

    setLayout(mainLayout);

    // Connect signals and slots
    connect(btnInsertNode, &QPushButton::clicked, this, &QtWidgetsApplication1::onInsertNodeClicked);
    connect(btnPreorder, &QPushButton::clicked, this, &QtWidgetsApplication1::onPreorderClicked);
    connect(btnInorder, &QPushButton::clicked, this, &QtWidgetsApplication1::onInorderClicked);
    connect(btnPostorder, &QPushButton::clicked, this, &QtWidgetsApplication1::onPostorderClicked);
    connect(btnLevelOrder, &QPushButton::clicked, this, &QtWidgetsApplication1::onLevelOrderClicked);
    connect(btnNewTree, &QPushButton::clicked, this, &QtWidgetsApplication1::onNewTreeClicked);
    connect(btnSum, &QPushButton::clicked, this, &QtWidgetsApplication1::onSumClicked);
    connect(btnHeight, &QPushButton::clicked, this, &QtWidgetsApplication1::onHeightClicked);

    // Initialize the Parent ComboBox
    updateParentComboBox();
}

// Destructor
QtWidgetsApplication1::~QtWidgetsApplication1()
{
    deleteTree(root);  // Clean up the tree
}

// Function to update the Parent ComboBox
void QtWidgetsApplication1::updateParentComboBox()
{
    cmbParentNode->clear();  // Clear previous entries
    cmbParentNode->addItem("Root");

    if (root != nullptr) {
        QQueue<TreeNode*> nodeList;
        nodeList.enqueue(root);

        // Traverse the tree and add nodes to the ComboBox
        while (!nodeList.isEmpty()) {
            TreeNode* current = nodeList.dequeue();
            cmbParentNode->addItem(QString::number(current->data));

            // Enqueue left and right children to be processed
            if (current->left) nodeList.enqueue(current->left);
            if (current->right) nodeList.enqueue(current->right);
        }
    }
}

// Insert Node Handler
void QtWidgetsApplication1::onInsertNodeClicked()
{
    bool ok;
    int value = txtNodeValue->text().toInt(&ok);
    if (!ok) {
        txtResult->setText("Please enter a valid integer.");
        return;
    }

    TreeNode* selectedParent = nullptr;
    QString selectedParentText = cmbParentNode->currentText();
    if (selectedParentText.isEmpty()) {
        txtResult->setText("Please select a parent node.");
        return;
    }

    if (selectedParentText == "Root") {
        if (root == nullptr) {
            root = new TreeNode(value);  // Set root if it doesn't exist
            txtResult->setText("Root node added: " + QString::number(value));
        }
        else {
            txtResult->setText("Root already exists.");
        }
    }
    else {
        int parentValue = selectedParentText.toInt();
        QQueue<TreeNode*> nodeList;
        nodeList.enqueue(root);
        while (!nodeList.isEmpty()) {
            TreeNode* current = nodeList.dequeue();
            if (current->data == parentValue) {
                selectedParent = current;
                break;
            }
            if (current->left) nodeList.enqueue(current->left);
            if (current->right) nodeList.enqueue(current->right);
        }

        if (!selectedParent) {
            txtResult->setText("Parent node not found.");
            return;
        }

        QString direction = cmbDirection->currentText();
        if (direction == "Left") {
            addLeftNode(selectedParent, value);
            txtResult->setText("Added node: " + QString::number(value) + " to the Left of node " + QString::number(parentValue));
        }
        else if (direction == "Right") {
            addRightNode(selectedParent, value);
            txtResult->setText("Added node: " + QString::number(value) + " to the Right of node " + QString::number(parentValue));
        }
    }

    updateParentComboBox();  // Refresh the Parent ComboBox
    update();  // Trigger repaint
}

// Add Left Child
void QtWidgetsApplication1::addLeftNode(TreeNode* node, int value)
{
    TreeNode* newNode = new TreeNode(value);
    node->left = newNode;
    newNode->parent = node;
}

// Add Right Child
void QtWidgetsApplication1::addRightNode(TreeNode* node, int value)
{
    TreeNode* newNode = new TreeNode(value);
    node->right = newNode;
    newNode->parent = node;
}

// Preorder Traversal
void QtWidgetsApplication1::preorder(TreeNode* node, QString& result)
{
    if (node == nullptr) return;
    result += QString::number(node->data) + " ";
    preorder(node->left, result);
    preorder(node->right, result);
}

// Inorder Traversal
void QtWidgetsApplication1::inorder(TreeNode* node, QString& result)
{
    if (node == nullptr) return;
    inorder(node->left, result);
    result += QString::number(node->data) + " ";
    inorder(node->right, result);
}

// Postorder Traversal
void QtWidgetsApplication1::postorder(TreeNode* node, QString& result)
{
    if (node == nullptr) return;
    postorder(node->left, result);
    postorder(node->right, result);
    result += QString::number(node->data) + " ";
}

// Level-order Traversal
void QtWidgetsApplication1::levelOrder(TreeNode* node, QString& result)
{
    if (node == nullptr) return;
    QQueue<TreeNode*> nodeList;
    nodeList.enqueue(node);
    while (!nodeList.isEmpty()) {
        TreeNode* current = nodeList.dequeue();
        result += QString::number(current->data) + " ";
        if (current->left) nodeList.enqueue(current->left);
        if (current->right) nodeList.enqueue(current->right);
    }
}

// Repainting the tree (empty implementation for now)
void QtWidgetsApplication1::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    if (root != nullptr) {
        drawTree(painter, root, 400, 50, 100);  // Draw the tree starting at position (400, 50)
    }
}

// Draw Tree Function
void QtWidgetsApplication1::drawTree(QPainter& painter, TreeNode* node, int x, int y, int dx)
{
    if (node == nullptr) return;

    painter.setPen(QPen(Qt::black));
    painter.setBrush(QBrush(Qt::lightGray));

    // Draw the current node
    painter.drawEllipse(x - 20, y - 20, 40, 40);
    painter.drawText(x - 10, y, QString::number(node->data));

    // Draw lines to children
    if (node->left) {
        painter.drawLine(x, y, x - dx, y + 100);
        drawTree(painter, node->left, x - dx, y + 100, dx / 2);
    }
    if (node->right) {
        painter.drawLine(x, y, x + dx, y + 100);
        drawTree(painter, node->right, x + dx, y + 100, dx / 2);
    }
}

void QtWidgetsApplication1::populateParentComboBox()
{
    cmbParentNode->clear();  // Clear previous entries
    cmbParentNode->addItem("Root");  // Add Root option as the first entry

    if (root != nullptr) {
        QQueue<TreeNode*> nodeList;
        nodeList.enqueue(root);

        // Traverse the tree and add nodes to the ComboBox
        while (!nodeList.isEmpty()) {
            TreeNode* current = nodeList.dequeue();
            cmbParentNode->addItem(QString::number(current->data));  // Add node value to ComboBox

            // Enqueue left and right children to be processed
            if (current->left) nodeList.enqueue(current->left);
            if (current->right) nodeList.enqueue(current->right);
        }
    }
}


// Create a New Tree
void QtWidgetsApplication1::onNewTreeClicked()
{
    deleteTree(root);
    root = nullptr;
    txtResult->clear();
    updateParentComboBox();
    update();  // Repaint the empty tree area
}

// Delete the Tree
void QtWidgetsApplication1::deleteTree(TreeNode* node)
{
    if (node == nullptr) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

// Get the Sum of All Nodes
void QtWidgetsApplication1::onSumClicked()
{
    int sum = sumOfNodes(root);
    txtResult->setText("Sum of nodes: " + QString::number(sum));
}

// Calculate the Sum of All Nodes
int QtWidgetsApplication1::sumOfNodes(TreeNode* node)
{
    if (node == nullptr) return 0;
    return node->data + sumOfNodes(node->left) + sumOfNodes(node->right);
}

// Get the Height of the Tree
void QtWidgetsApplication1::onHeightClicked()
{
    int height = treeHeight(root);
    txtResult->setText("Height of tree: " + QString::number(height));
}

// Calculate the Height of the Tree
int QtWidgetsApplication1::treeHeight(TreeNode* node)
{
    if (node == nullptr) return 0;
    int leftHeight = treeHeight(node->left);
    int rightHeight = treeHeight(node->right);
    return qMax(leftHeight, rightHeight) + 1;
}

// Preorder Traversal Button Clicked
void QtWidgetsApplication1::onPreorderClicked()
{
    QString result;
    preorder(root, result);
    txtResult->setText("Preorder: " + result);
}

// Inorder Traversal Button Clicked
void QtWidgetsApplication1::onInorderClicked()
{
    QString result;
    inorder(root, result);
    txtResult->setText("Inorder: " + result);
}

// Postorder Traversal Button Clicked
void QtWidgetsApplication1::onPostorderClicked()
{
    QString result;
    postorder(root, result);
    txtResult->setText("Postorder: " + result);
}

// Level-order Traversal Button Clicked
void QtWidgetsApplication1::onLevelOrderClicked()
{
    QString result;
    levelOrder(root, result);
    txtResult->setText("Level Order: " + result);
}