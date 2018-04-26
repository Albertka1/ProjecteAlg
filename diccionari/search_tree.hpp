#ifndef diccionari_search_tree_hpp
#define diccionari_search_tree_hpp

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

		BinaryNode * rotateL() {
			BinaryNode *leaf = R->L;
			BinaryNode *root = R;
			R->L = this;
			R = leaf;
			return root;
		}
		BinaryNode * rotateR() {
			BinaryNode *leaf = L->R;
			BinaryNode *root = L;
			L->R = this;
			L = leaf;
			return root;
		}

		void insert(paraula p) {
			if (p < n) {
				if (L != NULL) L->insert(p);
				else L = new BinaryNode(p);
			}
			else {
				if (R != NULL) R->insert(p);
				else R = new BinaryNode(p);
			}
		}

		int height() {
			int hl, hr;

			if (L != NULL) hl = L->height();
			else hl = 0;

			if (R != NULL) hr = R->height();
			else hr = 0;

			return max2(hl, hr) + 1;
		}
		BinaryNode * balance() {
			int hl, hr;

			if (L != NULL) {
				L = L->balance();
				hl = L->height();
			}
			else hl = 0;

			if (R != NULL) {
				R = R->balance();
				hr = R->height();
			}
			else hr = 0;

			BinaryNode * root = this;
			if (hl > hr + 1) root = rotateR()->balance();
			if (hl + 1 < hr) root = rotateL()->balance();

			return root;
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
			tree = tree->balance();
		}
		~BinarySearchTree() { if (tree != NULL) delete tree; }

		bool existeix(paraula p) const { return tree->exists(p); }

		operator std::string() const { return (std::string)*tree; }
	};

	class Treap : public Diccionari {};
}

#endif
