#ifndef diccionari_search_tree_hpp
#define diccionari_search_tree_hpp

#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

#include "paraula.hpp"
#include "diccionari.hpp"

namespace diccionari {
	inline int max2(int a, int b) {
		if (a > b) return a;
		return b;
	}

	class BTree : public Diccionari {};

	class BinarySearchTree : public Diccionari {
	private:
		class Node {
		public:
			paraula n;
			Node *L;
			Node *R;

			Node() { init(); }
			Node(paraula p) { init(); n = p; }
			~Node() {
				if (L != NULL) delete L;
				if (R != NULL) delete R;
			}

			void insert(paraula p) {
				if (p < n) {
					if (L != NULL) L->insert(p);
					else L = new Node(p);
				}
				else if (p > n) {
					if (R != NULL) R->insert(p);
					else R = new Node(p);
				}
			}

			static void tree_to_list(Node *root) {
				Node *tail = root;
				Node *rest = root->R;
				while (rest != NULL) {
					if (rest->L == NULL) {
						tail = rest;
						rest = rest->R;
					}
					else {
						Node *tmp = rest->L;
						rest->L = tmp->R;
						tmp->R = rest;
						rest = tmp;
						tail->R = tmp;
					}
				}
			}

			static void compress(Node *root, int count) {
				Node *scanner = root;
				for (int i = 0; i < count; i++) {
					Node *child = scanner->R;
					scanner->R = child->R;
					scanner = scanner->R;
					child->R = scanner->L;
					scanner->L = child;
				}
			}

			static void list_to_tree(Node *root, int size, int leaves) {
				compress(root, leaves);
				size = size - leaves;
				while (size > 1) {
					size = size / 2;
					compress(root, size);
				}
			}

			Node *DSW(int size) { // Day-Stout-Warren algorithm
				int leaves = size + 1 - (int)std::pow(2, (int)std::log2(size + 1));
				Node root;
				root.R = this;

				tree_to_list(&root);
				list_to_tree(&root, size, leaves);

				Node *r = root.R;
				root.R = NULL;
				return r;
			}

			bool exists(paraula p) {
				if (p == n) return true;
				if (p < n && L != NULL) return L->exists(p);
				if (p > n && R != NULL) return R->exists(p);
				return false;
			}

			operator std::string() {
				if (L == NULL && R == NULL) return std::to_string(n);

				std::string s = std::to_string(n) + "(";
				s += ((L != NULL) ? (std::string)*L : "NULL") + " ";
				s += ((R != NULL) ? (std::string)*R : "NULL") + ")";
				return s;
			}

		private:
			void init() {
				L = NULL;
				R = NULL;
			}
		};
		Node *tree;

	public:
		BinarySearchTree(const std::vector<paraula>& pars) : Diccionari() {
			if (pars.size() == 0) tree = NULL;
			else {
				tree = new Node(pars[0]);
				for (unsigned i = 1; i < pars.size(); ++i) {
					tree->insert(pars[i]);
				}
				tree = tree->DSW(pars.size());
			}
		}
		~BinarySearchTree() { if (tree != NULL) delete tree; }

		bool existeix(paraula p) const {
			if (tree == NULL) return false;
			return tree->exists(p);
		}

		operator std::string() const { return (std::string)*tree; }
	};

	class Treap : public Diccionari {
	private:
		class Node {};
		Node *tree;

		static int priority(paraula p) { return p % 10000; }
		static bool comp(paraula a, paraula b) { return priority(a) < priority(b); }

	public:
		Treap(const std::vector<paraula>& v) : Diccionari() {

		}

		operator std::string() const = 0;
		
		bool existeix(paraula p) const = 0;
	};
}

#endif
