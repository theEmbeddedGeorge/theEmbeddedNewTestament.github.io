#include <stdlib.h>
#include <stdio.h>
#include <string.h>


bstNode* BST_head = NULL;

int main() {
    int i;
    int array[] = {5,7,2,3,4,1,6,8,9};

    for (i = 0; i < 9; i++) {
        BST_head = insertVal(BST_head, array[i]);
    }

// test traverse
    TESTCASE("Case 1: In order print.", 0);
    traverseInOrder(BST_head);
    printf("\n============\n");
    TESTCASE("Case 1: In order print.", 1);

// test insertVal()
    TESTCASE("Case 2: Insert val into BST.", 0);
    BST_head = insertVal(BST_head, 9);
    traverseInOrder(BST_head);
    printf("\n============\n");

    BST_head = insertVal(BST_head, 0);
    traverseInOrder(BST_head);
    printf("\n============\n");

    BST_head = insertVal(BST_head, 17);
    traverseInOrder(BST_head);
    printf("\n============\n");
    TESTCASE("Case 2: Insert val into BST.", 1);

// test search and delete
    TESTCASE("Case 3: Search and delete.", 0);
    bstNode *test = NULL;

    ASSERT_M((search(BST_head, 9) != NULL), "Node should be found in the BST");
    ASSERT_M((search(BST_head, 1) != NULL), "Node should be found in the BST");
    ASSERT_M((search(BST_head, 100) == NULL), "Node should not be found in the BST");

// delete mid node

    traverseInOrder(BST_head);
    printf("\n============\n");

    test = search(BST_head, 1);
    if (test) {
        printf("Delete %d from BST\n", test->val);
        BST_head = deleteNode(BST_head, test);
        ASSERT_M((search(BST_head, 1) == NULL), "Node should Not be found in the BST");
    }

    traverseInOrder(BST_head);
    printf("\n============\n");

     test = search(BST_head, 17);
    if (test) {
        printf("Delete %d from BST\n", test->val);
        BST_head = deleteNode(BST_head, test);
        ASSERT_M((search(BST_head, 17) == NULL), "Node should Not be found in the BST");
    }

    traverseInOrder(BST_head);
    printf("\n============\n");

// delete head
    printf("Delete head %d from BST\n", BST_head->val);
    BST_head = deleteNode(BST_head, BST_head);
    traverseInOrder(BST_head);
    printf("\n============\n");
    TESTCASE("Case 3: Search and delete.", 1);

// test minvalue & maxval
    TESTCASE("Case 4: Min and Max.", 0);
    printf("Min val: %d  Max val: %d\n", minVal(BST_head), maxVal(BST_head));
    TESTCASE("Case 4: Min and Max.", 1);

    struct bstNode *root = NULL;
    
    root = insertVal(root, 20);
    insertVal(root,5);
    insertVal(root,1);
    insertVal(root,15);
    insertVal(root,9);
    insertVal(root,7);
    insertVal(root,12);
    insertVal(root,30);
    insertVal(root,25);
    insertVal(root,40);
    insertVal(root,45);
    insertVal(root,42);

    traverseInOrder(root);
    printf("\n============\n");

    return 0;
}

