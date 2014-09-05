#include <iostream>
#include <sstream>
#include <stack>
#include <queue>
#include <algorithm>

#include "algo_tree.h"

using namespace std;

// build a tree from input pre-order and in-order traversal sequence, assume that
// all node keys in the tree are unique, and, of course, the 2 vectors' size must be equal
template <class T>
typename BinaryTree<T>::Node* BinaryTree<T>::build_tree(const vector<T>& pre_order_seq, const vector<T>& in_order_seq)
{
	int N = pre_order_seq.size();
	if (in_order_seq.size() != N)
		throw BinaryTreeException(string("sequence's length not equal"));

	if (N == 0)
		return nullptr;

	T root_val = pre_order_seq[0];
	if (N == 1)
	{
		Node* root = new Node(root_val);
		return root;
	}

	// find the root in the in-order sequence, before it is the left child, after it is the right child
	typename vector<T>::const_iterator root_pos = std::find(in_order_seq.begin(), in_order_seq.end(), root_val);
	if (root_pos == in_order_seq.end())
		throw BinaryTreeException(string("The two sequences does not match to build a binary tree"));

	// get the in-order sequence of left and right child respectively.
	vector<T> left_inorder, right_inorder;
	std::copy(in_order_seq.begin(), root_pos, back_inserter(left_inorder));
	std::copy(++root_pos, in_order_seq.end(), back_inserter(right_inorder));

	// get the pre-order sequence of left and right child respectively
	vector<T> left_preorder, right_preorder;
	std::copy(pre_order_seq.begin() + 1, pre_order_seq.begin() + 1 + left_inorder.size(), back_inserter(left_preorder));
	std::copy(pre_order_seq.begin() + 1 + left_inorder.size(), pre_order_seq.end(), back_inserter(right_preorder));

	if (left_inorder.size() != left_preorder.size() || right_inorder.size() != right_preorder.size())
		throw BinaryTreeException(string("sub-sequence's length does not match"));

	Node* left_child = build_tree(left_preorder, left_inorder);
	Node* right_child = build_tree(right_preorder, right_inorder);

	Node* root = new Node(root_val);
	if (root == nullptr)
		throw BinaryTreeException(string("bad memory allocation"));

	root->left = left_child;
	root->right = right_child;

	return root;
}

template<class T>
void BinaryTree<T>::kill_tree(Node*& root)
{
	if (root)
	{
		kill_tree(root->left);
		kill_tree(root->right);

		delete root;
		root = nullptr;
	}
}

template<class T>
void BinaryTree<T>::PreOrderRecursive(const typename BinaryTree<T>::Node* root) const
{
	if (root)
	{
		cout << '|' << root->GetData() << "|  ";
		PreOrderRecursive(root->left);
		PreOrderRecursive(root->right);
	}
}

template<class T>
const vector<typename BinaryTree<T>::data_type> BinaryTree<T>::PreOrderNoneRecur1(void) const
{
	Node* root = m_root;
	vector<data_type> result;
	std::stack <Node*> stk;

	stk.push(root);
	while (!stk.empty())
	{
		root = stk.top();
		stk.pop();
		if (root)
		{
			result.push_back(root->GetData());
			stk.push(root->right);
			stk.push(root->left);
		}
	}
	return result;
}

template<class T>
void BinaryTree<T>::InOrderRecursive(const typename BinaryTree<T>::Node* root) const
{
	if (root)
	{
		InOrderRecursive(root->left);
		cout << '|' << root->GetData() << "|  ";
		InOrderRecursive(root->right);
	}
}

template<class T>
const vector<typename BinaryTree<T>::data_type> BinaryTree<T>::InOrderNoneRecur1(void) const
{
	vector<data_type> result;
	Node* root = m_root;
	if (root)
	{
		stack<Node*>stk;
		while (root || !stk.empty())
		{
			if (root)
			{
				stk.push(root);
				root = root->left;
			}
			else
			{
				root = stk.top();
				stk.pop();
				result.push_back(root->GetData());
				root = root->right;
			}
		}
	}
	return result;
}

template<class T>
const vector<typename BinaryTree<T>::data_type> BinaryTree<T>::InOrderNoneRecur2(void) const
{
	vector<data_type> result;

	Node* cur = m_root;
	Node* pre = nullptr;
	while (cur != nullptr)
	{
		if (cur->left == nullptr)
		{
			result.push_back(cur->GetData());
			cur = cur->right;
		}
		else
		{
			pre = cur->left;
			while (pre->right != nullptr && pre->right != cur)
				pre = pre->right;
			if (pre->right == nullptr)
			{
				pre->right = cur;
				cur = cur->left;
			}
			else
			{
				pre->right = nullptr;
				result.push_back(cur->GetData());
				cur = cur->right;
			}
		}
	}
	return result;
}

template<class T>
void BinaryTree<T>::PostOrderRecursive(const typename BinaryTree<T>::Node* root) const
{
	if (root)
	{
		PostOrderRecursive(root->left);
		PostOrderRecursive(root->right);
		cout << '|' << root->GetData() << "|  ";
	}
}

template<class T>
const vector<typename BinaryTree<T>::data_type> BinaryTree<T>::PostOrderNoneRecur1(void) const
{
	vector<data_type> result;
	if (m_root == nullptr)
		return result;

	stack<Node*> output;
	stack<Node*> temp;

	temp.push(m_root);

	while (!temp.empty())
	{
		Node* p = temp.top();
		temp.pop();
		output.push(p);
		if (p->left)
			temp.push(p->left);
		if (p->right)
			temp.push(p->right);
	}

	while (!output.empty())
	{
		result.push_back(output.top()->GetData());
		output.pop();
	}
	return result;
}

template<class T>
const vector<typename BinaryTree<T>::data_type> BinaryTree<T>::BreadthTraversal(void) const
{
	vector<data_type> result;

	queue<Node*> q;
	q.push(m_root);

	while (!q.empty())
	{
		Node* root = q.front();
		q.pop();
		if (root)
		{
			result.push_back(root->GetData());
			if (root->left != nullptr)
				q.push(root->left);
			if (root->right != nullptr)
				q.push(root->right);
		}
	}
	return result;
}

template <class T>
void BinaryTree<T>::find_key(typename BinaryTree<T>::Node* root, const typename BinaryTree<T>::key_type& key, typename BinaryTree<T>::Node*& p_node) const
{
	if (root)
	{
		if (root->GetKey() == key)
		{
			p_node = root;
			return;
		}
		find_key(root->left, key, p_node);
		find_key(root->right, key, p_node);
	}
}

template <class T>
typename BinaryTree<T>::Node* BinaryTree<T>::FindKey(const typename BinaryTree<T>::key_type& key) const
{
	Node* p_node = nullptr;
	find_key(m_root, key, p_node);

	return p_node;
}

template <class T>
void BinaryTree<T>::find_preorder_ancestors(typename BinaryTree<T>::Node* root, typename BinaryTree<T>::Node* parent, \
	const typename BinaryTree<T>::key_type& key, vector<typename BinaryTree<T>::Node*>& ancestors, bool& found) const
{
	if (root)
	{
		if (found == false)
			ancestors.push_back(parent);
		if (root->GetKey() == key)
		{
			found = true;
			return;
		}
		find_preorder_ancestors(root->left, root, key, ancestors, found);
		find_preorder_ancestors(root->right, root, key, ancestors, found);
	}
}

template<class T>
const vector<vector<typename BinaryTree<T>::key_type> > BinaryTree<T>::FindLeafPaths(const typename BinaryTree<T>::Node* root) const
{
	vector< vector<key_type> > paths;
	vector< vector<key_type> > lc_paths;
	vector< vector<key_type> > rc_paths;
	if (root == nullptr)
		return paths;

	if (root->left != nullptr)
		lc_paths = FindLeafPaths(root->left);
	if (root->right != nullptr)
		rc_paths = FindLeafPaths(root->right);

	for (unsigned int i = 0; i<lc_paths.size(); ++i)
		rc_paths.push_back(lc_paths[i]);

	int count = rc_paths.size();
	if (count == 0)
	{
		paths.emplace_back(1, root->GetKey());
	}
	else
	{
		for (int i = 0; i<count; ++i)
		{
			rc_paths[i].insert(rc_paths[i].begin(), root->GetKey());
			paths.emplace_back(rc_paths[i]);
		}
	}

	return paths;
}

template<class T>
bool BinaryTree<T>::IsBST(const typename BinaryTree<T>::Node* root) const
{
	if (root == nullptr)
		return true;

	if ((root->left  && root->left->GetData()>root->GetData()) || (root->right && root->right->GetData()<root->GetData()))
		return false;
	bool lc_result = IsBST(root->left);
	if (lc_result == false)
		return false;
	bool rc_result = IsBST(root->right);
	if (rc_result == false)
		return false;

	return true;
}

template<class T>
void BinaryTree<T>::get_euler_tour(typename BinaryTree<T>::Node* root, vector<key_type>& output) const
{
	if (!root)
		return;
	struct path_track
	{
		Node* r;
		bool l_visited;
		bool r_visited;
		path_track() : r(nullptr), l_visited(false), r_visited(false) {};
		path_track(Node* _r) : r(_r), l_visited(false), r_visited(false) {};
	};
	stack<path_track> stk;
	stk.push(path_track(root));

	path_track pt;
	while (!stk.empty())
	{
		pt = stk.top();
		stk.pop();
		output.push_back(pt.r->GetKey());		//pop the node from stack to be visited

		if (pt.r->left && pt.l_visited == false)	// there's a unvisited left child
		{
			pt.l_visited = true;
			stk.push(pt);			// save the return point
			stk.push(path_track(pt.r->left));	// push the left child into stack to be visited
			continue;
		}
		if (pt.r->right && pt.r_visited == false)	// there's a unvisited right child
		{
			pt.r_visited = true;
			stk.push(pt);
			stk.push(path_track(pt.r->right));	// push the right child into stack to be visited
			continue;
		}
	}
}

template<class T>
const vector<typename BinaryTree<T>::key_type> BinaryTree<T>::GetEulerTour(void) const
{
	vector<key_type> tour;
	get_euler_tour(m_root, tour);

	return tour;
}



// ------------------------------- Cartesian Tree Definition ----------------------------------- //

template<class T>
typename CartesianTree<T>::CartesianNode* CartesianTree<T>::build_cartesian_tree(const vector<T>& seq) const
{
	CartesianNode *root = nullptr;
	CartesianNode *current = nullptr;

	for (auto x : seq)
	{
		if (root == nullptr)
		{
			root = new CartesianNode(x);
			current = root;
		}
		else
		{
			CartesianNode* p = new CartesianNode(x);
			CartesianNode* q = current;
			if (p->GetKey() < q->GetKey())
			{
				while (q->GetParent()!=nullptr && p->GetKey() < q->GetParent()->GetKey())
					q = q->GetParent();
				if (q->GetParent() == nullptr)
				{
					root = p;
					p->left = q;
					q->parent = p;
					current = p;
				}
				else
				{
					p->parent = q->parent;
					q->parent = p;
					p->parent->right = p;
					p->left = q;
					current = p;
				}
			}
			else
			{
				q->right = p;
				p->parent = q;
				current = p;
			}
		}
	}
	return root;
}


/*--------------------------------------- End of Class Definition ---------------------------------------------*/

// A supporting function to extract data sequence from string input used by main()
template<class T>
vector<T> get_seq(const string& seq_str)
{
	vector<T> seq;

	if (seq_str.length() <= 0)
		return seq;

	istringstream isstr(seq_str);

	T t;
	while (isstr >> t)
		seq.push_back(t);

	return seq;
}

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------

The following data sequences

const char *pre_key_str  = "1 2 4 8 9 14 18 22 15 19 23 5 10 3 6 11 7 12 16 20 24 26 13 17 21 25";
const char *in_key_str = "8 4 18 22 14 9 15 19 23 2 10 5 1 6 11 3 12 20 26 24 16 7 13 17 21 25";

const char *pre_data_str = "aa bb dd hh ii nn rr vv oo ss ww ee jj cc ff kk gg ll pp tt xx zz mm qq uu yy";
const char *in_data_str  = "hh dd rr vv nn ii oo ss ww bb jj ee aa ff kk cc ll tt zz xx pp gg mm qq uu yy";

will construct a binary tree demonstrated as the following graph:

                                    ___________________ (1, aa) ___________________
                                   |                                               |
                       ________ (2, bb) ________________               ________ (3, cc) __________
                      |                                 |             |                           |
                __ (4, dd)_____                   __ (5, ee)       (6, ff)____             ___ (7, gg) _____
               |               |                 |                            |           |                 |
           (8, hh)     ___ (9, ii) ____      (10, jj)                     (11, kk)    (12, ll) _        (13, mm) _
                      |                |                                                        |                 |
               ___ (14, nn)      (15, oo) __                                            ____ (16, pp)          (17, qq) ___
              |                             |                                          |                                   |
          (18, rr) ___                   (19, ss) ___                              (20, tt) ___                         (21, uu) ___
                      |                              |                                         |                                    |
                  (22, vv)                        (23, ww)                             ___ (24, xx)                              (25, yy)
                                                                                      |
                                                                                   (26, zz)

-------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

int main()
{
	cout << "*--------------- binary tree operations for DataStruct1 -------------------*" << endl;

// The following 2 sequences are not legal sequences
//	vector<DataStruct1> pre_seq { { 1, "aa" }, { 2, "bb" }, { 3, "cc" }, { 4, "dd" }, { 5, "ee" }, { 6, "ff" }, { 7, "gg" }, { 8, "hh" }, { 9, "ii" }, { 10, "jj" }, { 11, "kk" }, { 12, "ll" }, \
//	{ 13, "mm" }, { 14, "nn" }, { 15, "oo" }, { 16, "pp" }, { 17, "qq" }, { 18, "rr" }, { 19, "ss" }, { 20, "tt" }, { 21, "uu" }, { 22, "vv" }, { 23, "ww" }, { 24, "xx" }, { 25, "yy" }, { 26, "zz" } };
	vector<DataStruct1> pre_seq { { 1, "aa" }, { 2, "bb" }, { 4, "dd" }, { 8, "hh" }, { 9, "ii" }, { 14, "nn" }, { 18, "rr" }, { 22, "vv" }, { 15, "oo" }, { 19, "ss" }, { 23, "ww" }, { 5, "ee" }, \
	{ 10, "jj" }, { 3, "cc" }, { 6, "ff" }, { 11, "kk" }, { 7, "gg" }, { 12, "ll" }, { 16, "pp" }, { 20, "tt" }, { 24, "xx" }, { 26, "zz" }, { 13, "mm" }, { 17, "qq" }, { 21, "uu" }, { 25, "yy" } };

	vector<DataStruct1> in_seq { { 8, "hh" }, { 4, "dd" }, { 18, "rr" }, { 22, "vv" }, { 14, "nn" }, { 9, "ii" }, { 15, "oo" }, { 19, "ss" }, { 23, "ww" }, { 2, "bb" }, { 10, "jj" }, { 5, "ee" }, \
	{ 1, "aa" }, { 6, "ff" }, { 11, "kk" }, { 3, "cc" }, { 12, "ll" }, { 20, "tt" }, { 26, "zz" }, { 24, "xx" }, { 16, "pp" }, { 7, "gg" }, { 13, "mm" }, { 17, "qq" }, { 21, "uu" }, { 25, "yy" } };

	BinaryTree<DataStruct1> bt1(pre_seq, in_seq);

	cout << "The pre-order sequence is: ";
	bt1.PreOrderRecursive(bt1.GetRoot());
	cout << endl << endl;
	vector<typename BinaryTree<DataStruct1>::data_type> pre_order1 = bt1.PreOrderNoneRecur1();
	cout << "The none-recur pre-order sequence1 is: ";
	for (unsigned int i = 0; i<pre_order1.size(); ++i)
		cout << "|" << pre_order1[i] << "|  ";
	cout << endl << endl;

	cout << "The in-order sequence is: ";
	bt1.InOrderRecursive(bt1.GetRoot());
	cout << endl;
	vector<typename BinaryTree<DataStruct1>::data_type> in_order1 = bt1.InOrderNoneRecur1();
	cout << "The none-recur in-order sequence1 is: ";
	for (unsigned int i = 0; i<in_order1.size(); ++i)
		cout << "|" << in_order1[i] << "|  ";
	cout << endl;
	vector<typename BinaryTree<DataStruct1>::data_type> in_order2 = bt1.InOrderNoneRecur2();
	cout << "The none-recur in-order sequence2 is: ";
	for (unsigned int i = 0; i<in_order2.size(); ++i)
		cout << "|" << in_order2[i] << "|  ";
	cout << endl << endl;

	cout << "The post-order sequence is: ";
	bt1.PostOrderRecursive(bt1.GetRoot());
	cout << endl;
	vector<typename BinaryTree<DataStruct1>::data_type> post_order1 = bt1.PostOrderNoneRecur1();
	cout << "The none-recur post-order sequence1 is: ";
	for (unsigned int i = 0; i<post_order1.size(); ++i)
		cout << "|" << post_order1[i] << "|  ";
	cout << endl << endl;

	vector<typename BinaryTree<DataStruct1>::data_type> breadth_seq = bt1.BreadthTraversal();
	cout << "The breadth traversal sequence is: ";
	for (unsigned int i = 0; i<breadth_seq.size(); ++i)
		cout << "|" << breadth_seq[i] << "|  ";
	cout << endl << endl;

	cout << endl << endl << "************************* PATH TO LEAF *************************" << endl << endl;
	vector< vector<BinaryTree<DataStruct1>::key_type> > paths = bt1.FindLeafPaths(bt1.GetRoot());
	for (unsigned int i = 0; i<paths.size(); ++i)
	{
		cout << "[ ";
		for (unsigned int j = 0; j<paths[i].size(); ++j)
		{
			cout << paths[i][j] << ' ';
		}
		cout << "]" << endl;
	}
	cout << endl;

	cout << endl << endl << "************************* Get Eular Tour *************************" << endl << endl;
	vector<typename BinaryTree<DataStruct1>::key_type> tour = bt1.GetEulerTour();
	cout << "[ ";
	for (unsigned int i = 0; i<tour.size(); ++i)
	{
		cout << tour[i] << ' ';
	}
	cout << "]" << endl << endl;


	cout << "*--------------- binary tree operations for DataStruct2 -------------------*" << endl;

	vector<DataStruct2> pre_seq2{1.1, 2.2, 4.4, 5.5, 7.7, 10.10, 3.3, 6.6, 8.8, 9.9};
	vector<DataStruct2> in_seq2{4.4, 2.2, 7.7, 10.10, 5.5, 1.1, 3.3, 8.8, 6.6, 9.9};
	BinaryTree<DataStruct2> bt2(pre_seq2, in_seq2);

	cout << "The in-order sequence is: ";
	bt2.InOrderRecursive(bt2.GetRoot());
	cout << endl << endl;

	cout << "The pre-order sequence is: ";
	bt2.PreOrderRecursive(bt2.GetRoot());
	cout << endl << endl;

	cout << "The post-order sequence is: ";
	bt2.PostOrderRecursive(bt2.GetRoot());
	cout << endl << endl;

	vector<typename BinaryTree<DataStruct2>::data_type> breadth_seq2 = bt2.BreadthTraversal();

	cout << "The breadth traversal sequence is: ";
	for (unsigned int i = 0; i<breadth_seq2.size(); ++i)
		cout << "|" << breadth_seq2[i] << "|  ";
	cout << endl;

	if (bt2.IsBST(bt2.GetRoot()) == true)
		cout << "This tree is a BST" << endl;
	else
		cout << "This tree is not a BST" << endl;

	cout << endl << "/*------------------------------- Build Cartesian Tree -----------------------------*/" << endl << endl;

	vector<DataStruct1> cartesian_seq{ { 5, "ee" }, { 3, "cc" }, { 8, "hh" }, { 4, "dd" }, { 1, "aa" }, { 7, "gg" }, { 6, "ff" }, { 2, "bb" }, { 9, "ii" } };
	CartesianTree<DataStruct1> ct(cartesian_seq);

	cout << "The in-order sequence of cartesian tree is: ";
	ct.InOrderRecursive(ct.GetRoot());
	cout << endl << endl;

	cout << "The pre-order sequence of cartesian tree is: ";
	ct.PreOrderRecursive(ct.GetRoot());
	cout << endl << endl;

	cout << "The post-order sequence cartesian tree is: ";
	ct.PostOrderRecursive(ct.GetRoot());
	cout << endl << endl;

	return 0;
}


