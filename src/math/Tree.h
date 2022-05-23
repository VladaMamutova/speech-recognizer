#ifndef TREE_H
#define TREE_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

namespace math {

template <typename KEY_TYPE, typename VALUE_TYPE>

class Tree
{
public:
	struct Node
	{
		KEY_TYPE key;
		VALUE_TYPE value;
		Node *left;
		Node *right;
	};

	Tree();
	~Tree();

	inline int getSize() { return size; }
	inline Node *getTree() { return root; }

	void add(KEY_TYPE key, VALUE_TYPE value);
	int calcHeight(Node *node);
	void leftRightTraversal(Node* node, vector<Node*>& nodes);

private:
	Node *root;
	int size;

	void deleteNode(Node *node);
};

} /* namespace math */

#endif /* TREE_H */
