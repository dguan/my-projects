#include <iostream>
#include <vector>
#include <memory>
#include <iterator>


// Some statistical variables
static int node_construction_cnt;
static int shared_node_construction_cnt;
static int node_destruction_cnt;
static int shared_node_destruction_cnt;

template<class T>
struct Node {
	T value;
	Node<T> *left;
	Node<T> *right;
	Node(T val) : value(val), left(nullptr), right(nullptr)	{ ++node_construction_cnt; };
	// Actually I do not need this desctructor, only to demonstrate that all nodes destructed correctly
	~Node() { ++node_destruction_cnt; };
};

template<class T>
struct SharedNode {
	T value;
	std::shared_ptr<SharedNode<T>> left;
	std::shared_ptr<SharedNode<T>> right;
	SharedNode(T val) : value(val), left(nullptr), right(nullptr)	{ ++shared_node_construction_cnt; };
	// Actually I do not need this desctructor, only to demonstrate that all shared nodes destructed correctly
	~SharedNode() { ++shared_node_destruction_cnt; };
};

// This function can be used on either type of trees
template<class RootPtr> void pre_order(RootPtr root)
{
	if (root)
	{
		std::cout << root->value << ", ";
		pre_order(root->left);
		pre_order(root->right);
	}
}
// This function can be used on either type of trees
template<class RootPtr> void in_order(RootPtr root)
{
	if (root)
	{
		in_order(root->left);
		std::cout << root->value << ", ";
		in_order(root->right);
	}
}

// Only normal trees need this function
template<class T> Node<T> *dup_tree(Node<T> *root)
{
	Node<T> *new_tree = nullptr;
	if (root)
	{
		new_tree = new Node<T>(root->value);
		new_tree->left = dup_tree(root->left);
		new_tree->right = dup_tree(root->right);
	}
	return new_tree;
}

// Shared trees does not need this function
template<class T> void kill_tree(Node<T> *root)
{
	if (root)
	{
		kill_tree(root->left);
		kill_tree(root->right);
		delete root;
	}
}

// Construct BST superset from N unique values
template<class T, class ConstRandIter>
std::vector<Node<T>*> create_bst(ConstRandIter node_value_begin, ConstRandIter node_value_end)
{
	std::vector<Node<T>*> trees;
	int n_nodes = std::distance(node_value_begin, node_value_end);
	if (n_nodes == 0)
	{
		trees.push_back(nullptr);
		return trees;
	}
	for (int i = 0; i < n_nodes; ++i)
	{
		auto left_subtrees = create_bst<T>(node_value_begin, std::next(node_value_begin, i));
		auto right_subtrees = create_bst<T>(std::next(node_value_begin, i + 1), node_value_end);
		for (auto l_tree :left_subtrees)
		{
			for (auto r_tree : right_subtrees)
			{
				Node<T> *root_ptr = new Node<T>(*std::next(node_value_begin, i));
				root_ptr->left = dup_tree(l_tree);
				root_ptr->right = dup_tree(r_tree);
				trees.push_back(root_ptr);
			}
		}
		for (auto l_tree : left_subtrees)
			kill_tree(l_tree);
		for (auto r_tree : right_subtrees)
			kill_tree(r_tree);
	}
	return trees;
}

// Construct shared BST superset from N unique values
template<class T, class ConstRandIter>
std::vector<std::shared_ptr<SharedNode<T>>> create_shared_bst(ConstRandIter node_value_begin, ConstRandIter node_value_end)
{
	std::vector<std::shared_ptr<SharedNode<T>>> trees;
	int n_nodes = std::distance(node_value_begin, node_value_end);
	if (n_nodes == 0)
	{
		trees.emplace_back(nullptr);
		return trees;
	}
	for (int i = 0; i < n_nodes; ++i)
	{
		auto left_subtrees = create_shared_bst<T>(node_value_begin, std::next(node_value_begin, i));
		auto right_subtrees = create_shared_bst<T>(std::next(node_value_begin, i + 1), node_value_end);
		for (auto l_tree : left_subtrees)
		{
			for (auto r_tree : right_subtrees)
			{
				std::shared_ptr<SharedNode<T>> root_ptr(new SharedNode<T>(*std::next(node_value_begin, i)));
				root_ptr->left = l_tree;
				root_ptr->right = r_tree;
				trees.push_back(root_ptr);
			}
		}
	}
	return trees;
}

// test and compare 2 types of BSTs
void test_normal_and_shared_BST()
{
	//std::vector<int> node_values{ 1, 2, 3 };
	std::vector<int> node_values{ 1, 2, 3, 4 };
	//std::vector<int> node_values{ 1, 2, 3, 4, 5 };
	//std::vector<int> node_values{ 1, 2, 3, 4, 5, 6 };

	std::cout << "***** Start constructing normal BSTs...... *****" << std::endl;
	std::vector<Node<int>*> trees = create_bst<int>(node_values.begin(), node_values.end());
	int n_trees = trees.size();
	for (Node<int> *root : trees)
	{
		std::cout << std::endl << "The PreOrder of this BST is: ";
		pre_order(root);
		std::cout << std::endl << "The InOrder of this BST is: ";
		in_order(root);
		std::cout << std::endl;
		kill_tree(root);	// have to manually kill these trees
	}

	std::cout << std::endl << std::endl << "***** Start constructing shared BSTs...... *****" << std::endl;
	std::vector<std::shared_ptr<SharedNode<int>>> shared_trees = create_shared_bst<int>(node_values.begin(), node_values.end());
	int n_shared_trees = shared_trees.size();
	for (auto root : shared_trees)
	{
		std::cout << std::endl << "The PreOrder of this shared BST is: ";
		pre_order(root);
		std::cout << std::endl << "The InOrder of this shared BST is: ";
		in_order(root);
		std::cout << std::endl;
	}

	std::cout << std::endl;
	std::cout << "Altogether " << n_trees << " BSTs, " << node_construction_cnt << " nodes constructed." << std::endl;
	std::cout << "Altogether " << n_shared_trees << " shared BSTs, " << shared_node_construction_cnt << " shared nodes constructed." << std::endl;
}

int main()
{
	test_normal_and_shared_BST();

	std::cout << "Altogether " << node_destruction_cnt << " nodes destructed." << std::endl;
	std::cout << "Altogether " << shared_node_destruction_cnt << " shared nodes destructed." << std::endl;

	return 0;
}
