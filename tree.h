
// A tree something

typedef struct node {
  unsigned char content; // Either content or children
  unsigned int count;
  struct node *left, *right;
} tree_node;

tree_node* tree_make_subtree(tree_node* left, tree_node* right);

tree_node* tree_make_leaf(char content, unsigned int count);

void tree_leaf_dump(tree_node* tree);
