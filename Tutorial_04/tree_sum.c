
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

typedef struct Node {
    int value;
    struct Node *left;
    struct Node *right;
} Node;

Node *createNode(int value) {
    Node *node = malloc(sizeof(Node));
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

long long treeSum(Node *root) {
    if (root == NULL)
        return 0;

    long long leftSum = 0, rightSum = 0;

    if (root->left && root->right) {
        #pragma omp task shared(leftSum) firstprivate(root)
        leftSum = treeSum(root->left);

        #pragma omp task shared(rightSum) firstprivate(root)
        rightSum = treeSum(root->right);

        #pragma omp taskwait
    } else if (root->left) {
        leftSum = treeSum(root->left);
    } else if (root->right) {
        rightSum = treeSum(root->right);
    }

    return root->value + leftSum + rightSum;
}

int main() {
    Node *root = createNode(1);
    root->left = createNode(2);
    root->right = createNode(3);
    root->left->left = createNode(4);
    root->left->right = createNode(5);
    root->right->left = createNode(6);
    root->right->right = createNode(7);

    long long sum = 0;

    #pragma omp parallel
    {
        #pragma omp single
        sum = treeSum(root);
    }

    printf("Sum of tree nodes = %lld\n", sum);

    return 0;
}
