#include <stdio.h>
#include <stdlib.h>

#define MAX_CHILDREN 256

typedef struct IPRangeNode {
    unsigned int ip_address;  // Store IP address as integer
    char *subnet_mask;
    struct IPRangeNode *children[MAX_CHILDREN];
} IPRangeNode;

typedef struct IPRangeTree {
    IPRangeNode *root;
} IPRangeTree;

IPRangeNode *create_ip_range_node(unsigned int ip_address, char *subnet_mask) {
    IPRangeNode *node = (IPRangeNode *)malloc(sizeof(IPRangeNode));
    if (node != NULL) {
        node->ip_address = ip_address;
        node->subnet_mask = subnet_mask;
        for (int i = 0; i < MAX_CHILDREN; i++) {
            node->children[i] = NULL;
        }
    }
    return node;
}

IPRangeTree *create_ip_range_tree() {
    IPRangeTree *tree = (IPRangeTree *)malloc(sizeof(IPRangeTree));
    if (tree != NULL) {
        tree->root = create_ip_range_node(0, ""); // Initialize root with 0 IP address
    }
    return tree;
}

void add_range(IPRangeTree *tree, unsigned int ip_address, char *subnet_mask) {
    IPRangeNode *current = tree->root;
    for (int level = 0; level < 32; level += 8) {
        int octet = (ip_address >> (24 - level)) & 0xFF;
        if (current->children[octet] == NULL) {
            current->children[octet] = create_ip_range_node(ip_address, subnet_mask);
        }
        current = current->children[octet];
    }
}

char *find_range(IPRangeTree *tree, unsigned int ip_address) {
    IPRangeNode *current = tree->root;
    for (int level = 0; level < 32; level += 8) {
        int octet = (ip_address >> (24 - level)) & 0xFF;
        if (current->children[octet] == NULL) {
            return NULL;
        }
        current = current->children[octet];
    }
    return current->subnet_mask;
}

void delete_tree(IPRangeNode *node) {
    if (node == NULL) {
        return;
    }
    for (int i = 0; i < MAX_CHILDREN; i++) {
        delete_tree(node->children[i]);
    }
    free(node);
}

int main() {
    IPRangeTree *tree = create_ip_range_tree();

    // Add IP address ranges
    add_range(tree, 0x0A0A0A00, "/24"); // 10.10.10.0/24
    add_range(tree, 0xAC100000, "/16"); // 172.16.0.0/16

    // Find IP address ranges
    printf("10.10.10.50: %s\n", find_range(tree, 0x0A0A0A32)); // Output: /24
    printf("172.16.10.50: %s\n", find_range(tree, 0xAC100A32)); // Output: /16

    // Free memory
    delete_tree(tree->root);
    free(tree);

    return 0;
}
