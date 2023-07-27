#include "Huffman.h"

Huffman::Huffman() {
  // your code here
}

Huffman::~Huffman() {
  // your code here
}

shared_ptr<freq_info> Huffman::CreateLeaf(char symbol, int count) {
  // Create a leaf node for the given symbol and count.
  shared_ptr<freq_info> leaf = make_shared<freq_info>();
  leaf->symbol = symbol;
  leaf->count = count;
  leaf->left = nullptr;
  leaf->right = nullptr;
  leaf->is_leaf = true;
  return leaf;
}

shared_ptr<freq_info> Huffman::CombineNodes(shared_ptr<freq_info> left, shared_ptr<freq_info> right) {
  // Combine two nodes to create an internal node.
  shared_ptr<freq_info> internal_node = make_shared<freq_info>();
  internal_node->left = left;
  internal_node->right = right;
  internal_node->is_leaf = false;
  // The symbol and count for the internal node are undefined, as it represents a combination of left and right nodes.
  return internal_node;
}

void Huffman::IncrementLookup(map<char, int>& lookup, char symbol) {
  // Increment the count associated with the symbol in the lookup table.
  lookup[symbol]++;
}

void Huffman::LoadQueue(const map<char, int>& lookup, tree_queue& q) {
  // Load the priority queue with leaf nodes from the lookup table.
  // Each leaf node represents a symbol and its frequency count.
  for (const auto& entry : lookup) {
    shared_ptr<freq_info> leaf = CreateLeaf(entry.first, entry.second);
    q.push(leaf);
  }
}

shared_ptr<freq_info> Huffman::BuildTree(tree_queue& q) {
  // Build the Huffman codec tree using the priority queue.
  // Repeatedly combine nodes with the lowest counts until only one node remains (the root).
  while (q.size() > 1) {
    shared_ptr<freq_info> left = q.top();
    q.pop();
    shared_ptr<freq_info> right = q.top();
    q.pop();
    shared_ptr<freq_info> combined = CreateInternalNode(left, right);
    q.push(combined);
  }
  return q.top(); // Return the root node of the Huffman tree.
}

void Huffman::traverseTree(shared_ptr<freq_info> node, string code, map<char, string>& encoding_table) {
  // Helper function for building the encoding table by traversing the Huffman codec tree.
  if (!node) {
    return;
  }
  if (node->is_leaf) {
    // If the current node is a leaf, store its symbol and its corresponding code in the encoding table.
    encoding_table[node->symbol] = code;
  }
  // Recursively traverse the left and right subtrees, appending LEFT_CHAR or RIGHT_CHAR to the code.
  traverseTree(node->left, code + LEFT_CHAR, encoding_table);
  traverseTree(node->right, code + RIGHT_CHAR, encoding_table);
}

map<char, string> Huffman::BuildEncodingTable(shared_ptr<freq_info> root) {
  // Build the lookup table that maps symbols in the Huffman codec tree to their corresponding binary codes.
  map<char, string> encoding_table;
  traverseTree(root, "", encoding_table); // Start the traversal from the root with an empty code.
  return encoding_table;
}

string Huffman::Encode(map<char, string> enc_table, string input) {
  // Encode the input string using the encoding lookup table.
  string encoded_string;
  for (char c : input) {
    // Append the binary code for each symbol in the input string.
    encoded_string += enc_table[c];
  }
  return encoded_string;
}

string Huffman::Decode(shared_ptr<freq_info> root, string input) {
  // Decode the encoded string using the Huffman codec tree.
  string decoded_string;
  shared_ptr<freq_info> current_node = root;

  for (char ch : input) {
    // Traverse the Huffman codec tree based on the input string's binary code.
    if (ch == LEFT_CHAR[0]) {
      // Move to the left child node if LEFT_CHAR is encountered.
      current_node = current_node->left;
    } else if (ch == RIGHT_CHAR[0]) {
      // Move to the right child node if RIGHT_CHAR is encountered.
      current_node = current_node->right;
    }

    // If the current node is a leaf node, append its symbol to the decoded string.
    if (current_node->is_leaf) {
      decoded_string += current_node->symbol;
      // Move back to the root for the next symbol.
      current_node = root;
    }
  }

  return decoded_string;
}

shared_ptr<freq_info> Huffman::CreateInternalNode(shared_ptr<freq_info> left, shared_ptr<freq_info> right) {
  // Create an internal node by combining two nodes.
  shared_ptr<freq_info> internal_node = make_shared<freq_info>();
  internal_node->count = left->count + right->count;
  internal_node->left = left;
  internal_node->right = right;
  internal_node->is_leaf = false;
  // The symbol for the internal node is undefined, as it represents a combination of left and right nodes.
  return internal_node;
}
