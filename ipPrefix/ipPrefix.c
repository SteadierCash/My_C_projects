#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define CHILDREN 2

typedef struct Node
{
    bool isPrefix;
    struct Node* children[CHILDREN];
} Node;

int add(unsigned int base, char* mask);
int del(unsigned int base, char* mask);
int check(unsigned int ip);

// Global root variable
Node* root = NULL;

int main() {
    //Create root node with children
    root = (Node*)malloc(sizeof(Node));
    root->isPrefix = false;
    for (int i = 0; i < CHILDREN; i++) {
        root->children[i] = NULL;
    }

    // Test adding and checking
    printf("----- 8 -----\n");
    add(0x0A0A0A0A, "8");
    printf("Mask for IP 10.10.10.1: %d\n", check(0x0A0A0A01));
    printf("Mask for IP 11.11.11.255: %d\n", check(0x0B0B0BFF));

    printf("----- 16 -----\n");
    add(0x0A0A0A00, "16");
    printf("Mask for IP 10.10.1.1: %d\n", check(0x0A0A0101));
    printf("Mask for IP 10.11.2.255: %d\n", check(0x0A0B02FF));

    printf("----- 24 -----\n");
    add(0x0A0A0A00, "24");
    printf("Mask for IP 10.10.10.1: %d\n", check(0x0A0A0A01));
    printf("Mask for IP 10.10.2.255: %d\n", check(0x0A0A02FF));

    printf("----- 32 -----\n");
    add(0x0A0A0A00, "32");
    printf("Mask for IP 10.10.10.0: %d\n", check(0x0A0A0A00));
    printf("Mask for IP 10.10.2.255: %d\n", check(0x0A0A02FF));

    printf("----- -1 -----\n");
    printf("Mask for IP 0.10.2.255: %d\n", check(0x000A02FF));
    printf("Mask for IP 255.10.2.255: %d\n", check(0xFF0A02FF));

    printf("----- Delete -----\n");
    printf("Mask for IP 10.10.10.1 before delete: %d\n", check(0x0A0A0A01));
    del(0x0A0A0A00, "24");
    printf("Mask for IP 10.10.10.1 after 1 delete: %d\n", check(0x0A0A0A01));
    del(0x0A0A0A00, "16");
    printf("Mask for IP 10.10.10.1 after 2 delete: %d\n", check(0x0A0A0A01));


    return 0;
}


int add(unsigned int base, char* mask){
    //validate mask value
    if (atoi(mask) > 32 || atoi(mask) < 0){
        printf("Invalid mask value\n");
        return -1;
    }
    
    //create nodes connected to base value 
    Node* current = root;
    int mask_value = atoi(mask);
    for (int i = 31; i >= 32 - mask_value; i--) {
        // printf("mask_value: %d\n", i);
        int bit = (base >> i) & 1;

        if (current->children[bit] == NULL) {
            current->children[bit] = (Node*)malloc(sizeof(Node));
            current->children[bit]->isPrefix = false;

            for (int j = 0; j < CHILDREN; j++) {
                current->children[bit]->children[j] = NULL;
            }
        }

        current = current->children[bit];

    }
    current->isPrefix = true;
}

int del(unsigned int base, char* mask){
    Node* current = root;
    Node* parent = NULL;
    int last_bit;

    for (int i = 31; i >= 32 - atoi(mask); i--) {
        int bit = (base >> i) & 1;

        // Move to the next level in the trie
        if (current->children[bit] != NULL){
            parent = current;
            current = current->children[bit];
            last_bit = bit;
        }
        else{
            // No node found, return without deleting
            return -1;
        }
    }

    // At this point, 'current' points to the node to delete or mark as non-prefix
    current->isPrefix = false;

    // Check if the node has no children, then delete it
    if (current->children[0] == NULL && current->children[1] == NULL){
        free(current);
        // Set the parent's pointer to NULL
        parent->children[last_bit] = NULL;
    }

    return 0;
}


int check(unsigned int ip){
    // Check if Trie is initialized
    if (root == NULL) {
        printf("Trie not initialized. Please initialize root node.\n");
        return -1;
    }

    // Initialize variables to track the length of the longest prefix match
    int longestMatch = -1;

    // Start traversal from the root node
    Node* current = root;
    for (int i = 31; i >= 0; i--) {
        int bit = (ip >> i) & 1;

        // Move to the next level in the trie
        if (current->children[bit] != NULL){
            current = current->children[bit];
            if (current->isPrefix == true) {
                longestMatch = 32 - i; // Update the length of the longest prefix match found so far
            }
        }
        else{
            break; // Exit the loop if there are no further nodes to traverse
        }
    }

    return longestMatch;
}