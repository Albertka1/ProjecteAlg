#ifndef diccionari_search_tree_hpp
#define diccionari_search_tree_hpp

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

	class BinaryNode {
	public:
		paraula n;
		BinaryNode *L;
		BinaryNode *R;

		BinaryNode() { init(); }
		BinaryNode(paraula p) { init(); n = p; }
		~BinaryNode() {
			if (L != NULL) delete L;
			if (R != NULL) delete R;
		}

		void insert(paraula p) {
			if (p < n) {
				if (L != NULL) L->insert(p);
				else L = new BinaryNode(p);
			}
			else if (p > n) {
				if (R != NULL) R->insert(p);
				else R = new BinaryNode(p);
			}
		}

		static void list(BinaryNode *root) {
			BinaryNode *tail = root;
			BinaryNode *rest = root->R;
			while (rest != NULL) {
				if (rest->L == NULL) {
					tail = rest;
					rest = rest->R;
				}
				else {
					BinaryNode *tmp = rest->L;
					rest->L = tmp->R;
					tmp->R = rest;
					rest = tmp;
					tail->R = tmp;
				}
			}
		}

		static void compress(BinaryNode *root, int count) {
			BinaryNode *scanner = root;
			for (int i = 0; i < count; i++) {
				BinaryNode *child = scanner->R;
				scanner->R = child->R;
				scanner = scanner->R;
				child->R = scanner->L;
				scanner->L = child;
			}
		}

		static void list_to_tree(BinaryNode *root, int size, int leaves) {
			compress(root, leaves);
			size = size - leaves;
			while (size > 1) {
				size = size / 2;
				compress(root, size);
			}
		}

		BinaryNode *DSW(int size) { // Day-Stout-Warren algorithm
			int leaves = size + 1 - (int)std::pow(2, (int)std::log2(size + 1));
			BinaryNode root;
			root.R = this;

			list(&root);
			list_to_tree(&root, size, leaves);

			BinaryNode *r = root.R;
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
			s += ((L != NULL) ? (std::string)*L : "N") + " ";
			s += ((R != NULL) ? (std::string)*R : "N") + ")";
			return s;
		}

	private:
		void init() {
			L = NULL;
			R = NULL;
		}
	};

	class BTree : public Diccionari {};

	class BinarySearchTree : public Diccionari {
	private:
		BinaryNode * tree;

	public:
		BinarySearchTree(const std::vector<paraula>& pars) : Diccionari() {
			tree = new BinaryNode(pars[0]);
			for (unsigned i = 1; i < pars.size(); ++i) {
				tree->insert(pars[i]);
			}
			tree = tree->DSW(pars.size());
		}
		~BinarySearchTree() { if (tree != NULL) delete tree; }

		bool existeix(paraula p) const { return tree->exists(p); }

		operator std::string() const { return (std::string)*tree; }
	};

	class Treap : public Diccionari {};
}

#endif
