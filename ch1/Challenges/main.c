#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    struct Node* from;
    struct Node* to;
    char* value;
} Node;


Node* get(int index, Node* root) {
  Node* node = root;
  for (int i = 0; i < index+1; i++) {
    if (node->to != NULL)
      node = node->to;
    else
      break;
  }

  return node;
}

void insert(char* string, int index, Node** rootPtr) {
  // Get node at index i - 1
  Node* priorNode = get(index - 1, *rootPtr);
  if (priorNode->to != NULL) {
    Node* nextNode = priorNode->to;
    Node node = {.from = priorNode, .to = nextNode, .value = string};

    priorNode->to = &node;
    if (nextNode != NULL) {
      nextNode->from = &node; 
    }
  } else {
    // If the prior node was the last
    Node* node = malloc(sizeof(Node));
    node->from = priorNode;
    node->to = NULL;
    node->value = malloc((strlen(string) + 1) * sizeof(char));
    strcpy(node->value, string);
    priorNode->to = node; 
  }
}

void delete(int index, Node** rootPtr) {
  Node* node = get(index, *rootPtr);
  Node* priorNode = node->from;

  // Re-wire around deleted node
  if (node->to != NULL) {
    Node* nextNode = node->to;
    nextNode->from = priorNode;
    priorNode->to = nextNode;
  } else {
    priorNode->to = NULL;
  }

  // Free the dynamically allocated memory
  free(node->value);
  free(node);
}

bool isTerminal(Node* node) {
  return node->to == NULL;
}

bool isRoot(Node* node) {
  return node->from == NULL && node->value == NULL;
}

int find(char* string, Node* root) {
  int result;
  int retval = -1;
  int index = 0;
  bool finished = false;
  Node* node = get(0, root);

  while (!finished) { 
    result = strcmp(string, node->value);

    // If the strings match
    if (result == 0) {
      finished = true;
      retval = index;
    } else {
      if (isTerminal(node)) {
        finished = true;
      } else {
        // Gets the next node without having to
        // start from the root. Hacky.
        node = get(1, node->from);
        index++;
      }
    }
  }

  return retval;
}

void displayContents(Node* node) {
  bool nodeIsRoot = isRoot(node);
  bool nodeIsTerminal = isTerminal(node);

  if (nodeIsRoot && nodeIsTerminal) {
    printf("H -> T");
  } else if (nodeIsTerminal) {
    printf("%s -> T", node->value);
  } else if (nodeIsRoot) {
    printf("H -> ");
    displayContents(node->to);
  } else {
    printf("%s -> ", node->value);
    displayContents(node->to);
  }
}

int main() {
  int numItems = 10;
  const int maxPayloadSize = 100;

  // Initialize root of doubly-linked list
  Node* root = malloc(sizeof(Node));
  root->from = NULL;
  root->to = NULL;
  root->value = NULL;
  Node** rootPtr = &root;

  // Populate
  for (int i = 0; i < numItems; i++) {
    char* payload = malloc((maxPayloadSize+1) * sizeof(char));
    sprintf(payload, "I <3 Susie %d,000,000", i+1); 
    insert(payload, i, rootPtr);
    free(payload);
  }

  printf("\n");
  displayContents(root);

  int index = find("taco bell", root);

  printf("\n\n");
  printf("Should be -1: ");
  printf("%d", index);

  index = find("I <3 Susie 3,000,000", root);

  printf("\n\n");
  printf("Should be 2: ");
  printf("%d", index);

  delete(numItems-1, rootPtr);
  delete(0, rootPtr);
  delete(5, rootPtr);
  delete(numItems-4, rootPtr);

  printf("\n\n");
  displayContents(root);
  printf("\n\n");

  return 0;
}
