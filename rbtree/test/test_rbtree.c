#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <rbtree.h>


static rbtree_node_t sentinel;

void get_rbtree_node_preorder(rbtree_node_t *node, rbtree_node_t *sentinel, int arr[], int *index) {
	arr[*index] = node->key;
	printf(" (val,color)=(%ld, %s) ", node->key, (node->color ? "r" : "b"));
	*index += 1;
	if (node->left != sentinel) {
		get_rbtree_node_preorder(node->left, sentinel, arr, index);
	} 
	if (node->right != sentinel) {
		get_rbtree_node_preorder(node->right, sentinel, arr, index);
	}
}

int *get_rbtree_preorder(rbtree_t *tree, int size) {
	int *arr = (int *)malloc(sizeof(int) * size);
	int index = 0;
	get_rbtree_node_preorder(tree->root, tree->sentinel, arr, &index);
	printf("\n");
	return arr;
}

void assert_rbtree(rbtree_t *tree, int arr[], int size) {
	int *tmp = get_rbtree_preorder(tree, size);
	for (int i = 0; i < size; i++) {
		if (arr[i] != tmp[i]) {
			fprintf(stderr, "arr[%d] = %d, tmp[%d] = %d, not equal\n", i, arr[i], i, tmp[i]);
			goto bad;
		}
	}
	free(tmp);
	return;

bad:
	fprintf(stderr, "arr=");
	for (int i = 0; i < size; i++) {
		fprintf(stderr, " %d ", arr[i]);
	}
	fprintf(stderr, "\n");

	
	fprintf(stderr, "tmp=");
	for (int i = 0; i < size; i++) {
		fprintf(stderr, " %d ", tmp[i]);
	}
	fprintf(stderr, "\n");

	free(tmp);
}

void test_rbtree_left_rotate(rbtree_t *tree, rbtree_node_t *node) {
	rbtree_left_rotate(tree, node);
}

void test_rbtree_right_rotate(rbtree_t *tree, rbtree_node_t *node) {
	rbtree_right_rotate(tree, node);
}

void test_rbtree_rotate() {
	rbtree_t tree;	
	rbtree_init(&tree, &sentinel, rbtree_insert_value);
    rbtree_node_t node[6] = {{0}};
    
    tree.root = &node[0];
    node[0].parent = &sentinel;
    node[0].key = 0;
    node[0].left = &node[1];
    node[0].right = &node[2];
    node[1].parent = &node[0];
    node[2].parent = &node[0];
    node[1].key = 1;
    node[2].key = 2;

    node[1].left = &node[3];
    node[1].right = &node[4];
    node[3].parent = &node[1];
    node[4].parent = &node[1];
    node[3].left = &sentinel;
    node[3].right = &sentinel;
    node[4].left = &sentinel;
    node[4].right = &sentinel;
    node[3].key = 3;
    node[4].key = 4;


    node[2].left = &node[5];
    node[2].right = &sentinel;
    node[5].parent = &node[2];
    node[5].key = 5;
    node[5].left = &sentinel;
    node[5].right = &sentinel;	


	test_rbtree_left_rotate(&tree, tree.root);
	int arr[6] = {2, 0, 1, 3, 4, 5};
	assert_rbtree(&tree, arr, 6);
	test_rbtree_right_rotate(&tree, tree.root);
	int arr2[6] = {0, 1, 3, 4, 2, 5};
	assert_rbtree(&tree, arr2, 6);
}


void test_rbtree_insert() {
	rbtree_t tree;
	rbtree_init(&tree, &sentinel, rbtree_insert_value);

	rbtree_node_t nodes[6] = {{0}};
	for (int i = 0; i < 6; i++) {
		nodes[i].key = i;
		rbtree_insert(&tree, &nodes[i]);
	}
	int arr[6] = {1, 0, 3, 2, 4, 5};
	assert_rbtree(&tree, arr, 6);
}

void test_rbtree_delete() {
	rbtree_t tree;
    rbtree_init(&tree, &sentinel, rbtree_insert_value);

    rbtree_node_t nodes[6] = {{0}};
    for (int i = 0; i < 6; i++) {
        nodes[i].key = i;
        rbtree_insert(&tree, &nodes[i]);
    }   
    int arr[6] = {1, 0, 3, 2, 4, 5}; 
    assert_rbtree(&tree, arr, 6);

	rbtree_delete(&tree, &nodes[0]);
	int arr2[5] = {3, 1, 2, 4, 5};
	assert_rbtree(&tree, arr2, 5);
}

int main() {
	test_rbtree_rotate();
	test_rbtree_insert();
	test_rbtree_delete();
}
