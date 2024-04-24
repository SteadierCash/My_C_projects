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

int add(unsigned int base, char *mask);
int del(unsigned int base, char mask);
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

    // Add some IP addresses
    // Add IP addresses with different masks
    // 10.10.10.0 -> 0x0A0A0A00
    // 10.10.0.0 -> 0x0A0A0000
    // 10.0.0.0 -> 0x0A000000
    // 10.10.1.1 -> 0x0A0A0101
    // 10.10.2.255 -> 0x0A0A02FF
    add(0x0A0A0A0A, "8"); 
    printf("Mask for IP 10.10.10.1: %d\n", check(0x0A0F0B0A));
    printf("Mask for IP 10.11.11.255: %d\n", check(0x0F0A0AFF));
    printf("----- 16 -----\n");

    add(0x0A0A0A00, "16"); 
    printf("Mask for IP 10.10.1.1: %d\n", check(0x0A0A0101));
    printf("Mask for IP 10.10.2.255: %d\n", check(0x0A0F02FF));
    printf("----- 24 -----\n");

    add(0x0A0A0A00, "24"); 
    printf("Mask for IP 10.10.1.1: %d\n", check(0x0A0A0A01));
    printf("Mask for IP 10.10.2.255: %d\n", check(0x0A0A02FF));

    printf("----- 32 -----\n");
    add(0x0A0A0A00, "32"); 
    printf("Mask for IP 10.10.1.1: %d\n", check(0x0A0A0A00));
    printf("Mask for IP 10.10.2.255: %d\n", check(0x0A0F02FF));
    

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


int del(unsigned int base, char mask){
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


// unsigned int ip_address = 0x0A0A0A00;

//     // Extract octets using bitwise operations
//     int octet1 = (ip_address >> 24) & 0xFF;
//     int octet2 = (ip_address >> 16) & 0xFF;
//     int octet3 = (ip_address >> 8) & 0xFF;
//     int octet4 = ip_address & 0xFF;

//     printf("10.10.10.50: %d.%d.%d.%d\n", octet1, octet2, octet3, octet4);