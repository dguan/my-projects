#ifndef ALGO_TREE_HEADER
#define ALGO_TREE_HEADER

#include <vector>
#include <string>
#include <exception>
#include <stdexcept>

#include "data_def.h"

template<class T>
class BinaryTree
{
public:
	typedef typename T::key_type key_type;
	typedef typename T::data_type data_type;

	struct Node
	{
		T data;
		Node* left;
		Node* right;

		Node() : data(T()), left(nullptr), right(nullptr) {};
		Node(const T& d) : data(d), left(nullptr), right(nullptr) {};
		//explicit Node(const key_type& k, const data_type& d) : data(k, d), left(nullptr), right(nullptr) {};

		const key_type GetKey(void) const { return data.get_key(); };
		const data_type GetData(void) const { return data.get_data(); };

		void SetData(data_type d) { data.set_data(d); };
	};

	class BinaryTreeException : public std::runtime_error
	{
	public:
		BinaryTreeException(const std::string& msg) : std::runtime_error(msg) {};
	};

	BinaryTree() { m_root = nullptr; };
	BinaryTree(const std::vector<T>& pre_seq, const std::vector<T>& in_seq) { m_root = build_tree(pre_seq, in_seq); };
	virtual ~BinaryTree() { kill_tree(m_root); m_root = nullptr; };
	virtual const Node* GetRoot(void) const { return m_root; };

	void PreOrderRecursive(const Node* root) const;
	void InOrderRecursive(const Node* root) const;
	void PostOrderRecursive(const Node* root) const;

	const std::vector<data_type> PreOrderNoneRecur1(void) const;
	const std::vector<data_type> PreOrderNoneRecur2(void) const;
	const std::vector<data_type> InOrderNoneRecur1(void) const;
	const std::vector<data_type> InOrderNoneRecur2(void) const;
	const std::vector<data_type> PostOrderNoneRecur1(void) const;
	const std::vector<data_type> PostOrderNoneRecur2(void) const;

	const std::vector<data_type> BreadthTraversal(void) const;

	Node* FindKey(const key_type& key) const;

	const std::vector< std::vector<key_type> > FindLeafPaths(const Node* root) const;
	bool IsBST(const Node* root) const;

	const std::vector<key_type> GetEulerTour(void) const;

protected:
	Node* build_tree(const std::vector<T>& pre_seq, const std::vector<T>& in_seq);
	void kill_tree(Node*& root);

	void find_key(Node* root, const key_type& key, Node*& node) const;
	void find_preorder_ancestors(Node* root, Node* parent, const key_type& key, std::vector<Node*>& ancestors, bool& found) const;
	void get_euler_tour(Node* root, std::vector<key_type>& output) const;

private:
	Node* m_root;
};


template<class T>
class CartesianTree : public BinaryTree<T>
{
public:
	typedef typename BinaryTree<T>::Node BaseNode;
	struct CartesianNode : public BaseNode
	{
		CartesianNode* parent;

		CartesianNode() : BaseNode(), parent(nullptr) {};
		CartesianNode(const T& t) : BaseNode(t), parent(nullptr) {};

		CartesianNode* GetParent(void) const { return parent; };
		//bool operator < (const CartesianNode& a) const { return this->GetKey() < a.GetKey(); };
	};

	CartesianTree () : BinaryTree<T>(), m_root(nullptr) {};
	CartesianTree(const std::vector<T>& seq) { m_root = build_cartesian_tree(seq); };

	CartesianNode* GetRoot(void) const { return m_root; };

protected:
	CartesianNode* build_cartesian_tree(const std::vector<T>& seq) const;

private:
	CartesianNode* m_root;
};

/*
template<class T>
class BST : public BinaryTree<T>
{
};

template<class T>
class RBT : public BinaryTree<T>
{
};
*/

#endif
