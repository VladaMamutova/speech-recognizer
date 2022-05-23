#include "Tree.h"

namespace math {

template <typename KEY_TYPE, typename VALUE_TYPE>
Tree<KEY_TYPE, VALUE_TYPE>::Tree()
{
	root = NULL;
	size = 0;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
Tree<KEY_TYPE, VALUE_TYPE>::~Tree()
{
	deleteNode(root);
}

template <typename KEY_TYPE, typename VALUE_TYPE>
void Tree<KEY_TYPE, VALUE_TYPE>::add(KEY_TYPE key, VALUE_TYPE value)
{
	Node *node = new Node; // включаемый элемент
	node->key = key;
	node->value = value;
	node->left = node->right = NULL;

	if (!root)
	{ // Добавить элемент в корень дерева.
		root = node;
		size = 1;
	}
	else
	{
		Node *temp = root;
		bool push = false; // Признак добавления элемента в дерево.

		while (!push)
		{ // Пока элемент не добавлен в дерево, ищем место для вставки.

			// Если нововведённый элемент меньше текущего, то добавляем его в левую ветвь.
			if (node->value < temp->value)
			{
				if (temp->left == NULL)
				{ // Добавить элемент в левую ветвь.
					push = true;
					++size;
					temp->left = node;
				}
				else
				{
					temp = temp->left; // Двигаемся по левой ветви.
				}
			}
			else
			{ // В противном случае (элемент больше или равен текущему) - в правую.
				if (temp->right == NULL)
				{ // Добавить элемент в правую ветвь.
					push = true;
					++size;
					temp->right = node;
				}
				else
				{
					temp = temp->right; // Двигаемся по правой ветви.
				}
			}
		} // while (!push)
	}
}

template <typename KEY_TYPE, typename VALUE_TYPE>
void Tree<KEY_TYPE, VALUE_TYPE>::leftRightTraversal(
	Tree<KEY_TYPE, VALUE_TYPE>::Node* node,
	vector<Tree<KEY_TYPE, VALUE_TYPE>::Node*>& nodes)
{
	if (node) {
		leftRightTraversal(node->left, nodes);
		nodes.push_back(node);
		leftRightTraversal(node->right, nodes);
	}
}

template <typename KEY_TYPE, typename VALUE_TYPE>
int Tree<KEY_TYPE, VALUE_TYPE>::calcHeight(Tree<KEY_TYPE, VALUE_TYPE>::Node *node)
{
	if (!node)
	{
		return 0;
	}
	else
	{
		int leftHeight = calcHeight(node->left);
		int rightHeight = calcHeight(node->right);

		return leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
	}
}

template <typename KEY_TYPE, typename VALUE_TYPE>
void Tree<KEY_TYPE, VALUE_TYPE>::deleteNode(Node *node)
{
	if (node)
	{
		if (node->left)
			deleteNode(node->left);
		if (node->right)
			deleteNode(node->right);
		delete node;
		node = NULL;
	}
}

template class Tree<string, double>;

} /* namespace math */
