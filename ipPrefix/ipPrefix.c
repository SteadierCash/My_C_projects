#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


#define CHILDREN 2


typedef struct Node
{
    bool isPrefix;
    struct Node* children[CHILDREN];
} Node;


//Functions Prototypes
int add(unsigned int base, char* mask);
int del(unsigned int base, char* mask);
char* check(unsigned int ip);
void tests();
unsigned int convert_ip(char* s);


// Global root variable
Node* root = NULL;

int main() {
    //Create root node with children
    root = (Node*)malloc(sizeof(Node));

    // Check if Trie is initialized
    if (root == NULL) {
        printf("Trie not initialized. Please initialize root node.\n");
        return -1;
    }

    root->isPrefix = false;
    for (int i = 0; i < CHILDREN; i++) {
        root->children[i] = NULL;
    }
    
    //Conduct tests
    tests();

    return 0;
}


int add(unsigned int base, char* mask){
    // Ignore first character
    if (strlen(mask) == 0){
        printf("Invalid mask value\n");
        return -1;
    }

    int mask_value = atoi(mask + 1);

    //validate mask value
    if (mask_value > 32 || mask_value < 0){
        printf("Invalid mask value\n");
        return -1;
    }
    
    //create nodes connected to base value 
    Node* current = root;

    for (int i = 31; i >= 32 - mask_value; i--) {

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
    // Ignore first character
    int mask_value = atoi(mask + 1);

    //Set current node for traveling through trie 
    //Set parent node in case children node will be deleted
    Node* current = root;
    Node* parent = NULL;
    int last_bit;

    for (int i = 31; i >= 32 - mask_value; i--) {
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

    // Check if the node has no children. If so delete it
    if (current->children[0] == NULL && current->children[1] == NULL){
        free(current);

        // Set the parent's pointer to NULL
        parent->children[last_bit] = NULL;
    }

    return 0;
}


char* check(unsigned int ip){
    // Initialize variable to track the length of the longest prefix
    int longestMatch = -1;

    Node* current = root;
    for (int i = 31; i >= 0; i--) {
        int bit = (ip >> i) & 1;

        // Move to the next level
        if (current->children[bit] != NULL){
            current = current->children[bit];
            if (current->isPrefix == true) {
                // Update the length of the longest prefix found
                longestMatch = 32 - i; 
            }
        }
        else{
            break;
        }
    }

    // allocate memory for answer as a string 
    char* str = (char*)malloc(4 * sizeof(char));
    
    // if longestMatch != -1 add '/' to result format
    if (longestMatch != -1){

        // sprintf terminates string with \0 value
        sprintf(str, "/%d", longestMatch);
    }
    else{
        sprintf(str, "%d", longestMatch);
    }

    return str;
}


void tests(){
    printf("----- add 32.64.128.0/20 -----\n");
    add(convert_ip("32.64.128.0"), "/20");
    printf("Mask for IP 32.64.128.0 :  %s\n", check(convert_ip("32.64.128.0")));
    printf("Mask for IP 32.64.140.10:  %s\n", check(convert_ip("32.64.140.10")));
    printf("Mask for IP 32.64.143.255: %s\n", check(convert_ip("32.64.143.255")));
    printf("\n");

    printf("----- add 10.20.0.0/16 -----\n");
    add(convert_ip("10.20.0.0"), "/16");
    printf("Mask for IP 10.20.0.0:      %s\n", check(convert_ip("10.20.0.0")));
    printf("Mask for IP 10.20.100.100:  %s\n", check(convert_ip("10.20.100.100")));
    printf("Mask for IP 10.20.255.255:  %s\n", check(convert_ip("10.20.255.255")));
    printf("\n");

    printf("----- add 10.10.10.10/8 -----\n");
    add(convert_ip("10.10.10.10"), "/8");
    printf("Mask for IP 10.11.11.1:      %s\n", check(convert_ip("10.11.11.1")));
    printf("Mask for IP 255.255.255.255: %s\n", check(convert_ip("255.255.255.255")));
    printf("\n");

    printf("----- add 10.10.10.10/18 -----\n");
    add(convert_ip("10.10.10.10"), "/18");
    printf("Mask for IP 10.10.63.254: %s\n", check(convert_ip("10.10.63.254")));
    printf("Mask for IP 10.11.10.255: %s\n", check(convert_ip("10.11.10.255")));
    printf("\n");

    printf("----- add 10.10.10.10/24 -----\n");
    add(convert_ip("10.10.10.10"), "/24");
    printf("Mask for IP 10.10.10.1:  %s\n", check(convert_ip("10.10.10.1")));
    printf("Mask for IP 10.10.2.255: %s\n", check(convert_ip("10.10.2.255")));
    printf("\n");

    printf("----- add 10.10.10.10/32 -----\n");
    add(convert_ip("10.10.10.10"), "/32");
    printf("Mask for IP 10.10.10.10:  %s\n", check(convert_ip("10.10.10.10")));
    printf("Mask for IP 10.10.10.255: %s\n", check(convert_ip("10.10.10.255")));
    printf("\n");

    printf("----- Wrong addresses -----\n");
    printf("Mask for IP 0.10.2.255:   %s\n", check(convert_ip("0.10.2.255")));
    printf("Mask for IP 255.10.2.255: %s\n", check(convert_ip("255.10.2.255")));
    printf("Mask for IP 0.0.0.0:      %s\n", check(convert_ip("0.0.0.0")));
    printf("\n");

    printf("----- Delete -----\n");
    printf("Mask for IP 10.10.10.1 before delete:         %s\n", check(convert_ip("10.10.10.1")));

    del(convert_ip("10.10.10.1"), "/24");
    printf("Mask for IP 10.10.10.1 after delete mask /24: %s\n", check(convert_ip("10.10.10.1")));

    del(convert_ip("10.10.10.1"), "/18");
    printf("Mask for IP 10.10.10.1 after delete mask /18: %s\n", check(convert_ip("10.10.10.1")));

    del(convert_ip("10.10.10.1"), "/8");
    printf("Mask for IP 10.10.10.1 after delete mask /8:  %s\n", check(convert_ip("10.10.10.1")));

    printf("\n----- add 10.10.10.10/100 -----\n");
    add(convert_ip("10.10.10.10"), "/100");

    printf("\n----- add 10.10.10.10 -----\n");
    add(convert_ip("10.10.10.10"), "");

    printf("\n----- add 1000.10.10.10/10 -----\n");
    add(convert_ip("1000.10.10.10"), "/10");

    printf("\n----- add 10.10.10/10 -----\n");
    add(convert_ip("10.10.10"), "/10");

    printf("\n----- add 10.10.10/10 -----\n");
    add(convert_ip("10.10.10"), "/10");
}


// convert string of ip format to int 
unsigned int convert_ip(char* s){
    int i = 0;

    int octets[4] = {0};
    int octets_cnt = 0;

    char octet[4];
    int octet_cnt = 0;

    while (s[i] != '\0'){
        if (s[i] == '.'){
            octet[octet_cnt] = '\0';
            octet_cnt = 0;
            int value = atoi(octet);

            //check if octet is correct
            if (value < 0 || value > 255) {
                printf("Invalid IP address: %s\n", s);
                return 0;
            }
            octets[octets_cnt++] = value;

        }
        else{
            octet[octet_cnt++] = s[i];
        }

        i++;
    }

    // Check if we have exactly four octets
    if (octets_cnt != 3) {
        printf("Invalid IP address: %s\n", s);
        return 0; 
    }

    //For the last octet
    octet[octet_cnt] = '\0'; 
    int value = atoi(octet);
    if (value < 0 || value > 255) {
        printf("Invalid IP address: %s\n", s);
        return 0; 
    }
    octets[octets_cnt++] = value;

    int swifts[] = {24, 16, 8, 0};
    unsigned int result = 0;

    for (int i=0; i < 4; i++){
        result = (octets[i] << swifts[i]) | result;
    } 

    return result;
}