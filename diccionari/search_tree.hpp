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

	class BinarySearchTree : public Diccionari {
	private:
		class Node {
		public:
			paraula key;
			Node *L;
			Node *R;

			Node() {}
			Node(paraula p) { key = p; }
			~Node() {
				if (L != NULL) delete L;
				if (R != NULL) delete R;
			}

			void insert(paraula p) {
				if (p < key) {
					if (L != NULL) L->insert(p);
					else L = new Node(p);
				}
				else if (p > key) {
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
				if (p == key) return true;
				if (p < key && L != NULL) return L->exists(p);
				if (p > key && R != NULL) return R->exists(p);
				return false;
			}

			unsigned split(const std::vector<paraula>& lot, unsigned a, unsigned b) {
				if (a == b) return a;
				if (lot[b] >= key) return b;
				if (lot[a] <= key) return a;

				unsigned pivot = (a + b) / 2;
				if (lot[pivot] < key)
					return split(lot, pivot, b);
				else
					return split(lot, a, pivot);
			}

			void set_search(const std::vector<paraula>& lot, std::vector<bool>& trobats, unsigned a, unsigned b) {
				unsigned pivot = split(lot, a, b); // key <= lot[pivot] < R->key
				if (L != NULL)
					L->set_search(lot, trobats, a, pivot);
				if (R != NULL)
					R->set_search(lot, trobats, a, pivot);
				if (lot[pivot] == key) trobats[pivot] = true;
			}

			void secciona(paraula p, Node *& l, Node *& r) {
				if (p < key) {
					l = L;
					r = this;
					if (L != NULL) L->secciona(p, l, r);
				}
				else if (p > key) {
					l = this;
					r = R;
					if (R != NULL) R->secciona(p, l, r);
				}
				else {
					l = this; r = this;
				}
			}

			std::vector<bool> q_search(std::vector<paraula>& lot) {
				paraula pivot = lot[0];
				std::vector<bool> trobats   = std::vector<bool>();
				std::vector<paraula> vLeft  = std::vector<paraula>();
				std::vector<paraula> vRight = std::vector<paraula>();

				trobats.reserve(lot.size());

				for (unsigned i = 0; i < lot.size(); ++i) {
					if (lot[i] < pivot) vLeft.push_back(lot[i]);
					else vRight.push_back(lot[i]);
				}

				Node *nLeft = this, *nRight = this;
				secciona(pivot, nLeft, nRight);

				if (nLeft != NULL && vLeft.size() > 0)
					trobats = nLeft->q_search(vLeft);

				trobats.push_back(nLeft == nRight);

				if (nRight != NULL && vRight.size() > 0) {
					std::vector<bool> tmp = nRight->q_search(vRight);
					trobats.insert(trobats.end(), tmp.begin(), tmp.end());
				}

				lot = vLeft;
				lot.push_back(pivot);
				lot.insert(lot.end(), vRight.begin(), vRight.end());

				return trobats;
			}

			operator std::string() {
				if (L == NULL && R == NULL) return std::to_string(key);

				std::string s = std::to_string(key) + "(";
				s += ((L != NULL) ? (std::string)*L : "NULL") + " ";
				s += ((R != NULL) ? (std::string)*R : "NULL") + ")";
				return s;
			}
		};
		Node *tree;

	public:
		BinarySearchTree(const std::vector<paraula>& v) : Diccionari() {
			if (v.size() == 0) tree = NULL;
			else {
				tree = new Node(v[0]);
				for (unsigned i = 1; i < v.size(); ++i) {
					tree->insert(v[i]);
				}
				tree = tree->DSW(v.size());
			}
		}
		~BinarySearchTree() { if (tree != NULL) delete tree; }

		operator std::string() const { return (std::string)*tree; }

		bool existeix(paraula p) const {
			if (tree == NULL) return false;
			return tree->exists(p);
		}

		bool optimitza_lot() const { return true; }
		std::vector<bool> existeix_lot(std::vector<paraula>& lot) const {
			return tree->q_search(lot);
		}

		bool optimitza_lot_ordenat() const { return true; }
		std::vector<bool> existeix_lot(const std::vector<paraula>& lot) const {
			std::vector<bool> trobats = std::vector<bool>(lot.size(), false);
			tree->set_search(lot, trobats, 0, lot.size()-1);
			return trobats;
		}
	};

	class Treap : public Diccionari {
	private:
		typedef int prio_type;
		static prio_type priority(paraula p) { return p % 10000; }
		static bool comp_p(paraula a, paraula b) { return priority(a) < priority(b); }
		void init(std::vector<paraula>& v) {
			if (v.size() == 0) tree = NULL;
			else {
				sort(v.begin(), v.end(), comp_p);
				tree = new Node(v[0]);
				for (unsigned i = 1; i < v.size(); ++i) {
					tree->insert(v[i]);
				}
			}
		}

		class Node {
		public:
			paraula key;
			prio_type prio;
			Node *L, *R;

			Node() {}
			Node(paraula p) { key = p; prio = priority(p); }
			~Node() {
				if (L != NULL) delete L;
				if (R != NULL) delete R;
			}

			void insert(paraula p) {
				if (p < key) {
						if (L != NULL) L->insert(p);
						else L = new Node(p);
				}
				else if (p > key) {
					if (R != NULL) R->insert(p);
					else R = new Node(p);
				}
			}

			bool exists(paraula p, prio_type pr) {
				if (pr > prio) return false;
				if (p < key) {
					if (L != NULL) return L->exists(p, pr);
					else return false;
				}
				else if (p > key) {
					if (R != NULL) return R->exists(p, pr);
					else return false;
				}
				else return true;
			}
			bool exists(paraula p) { return exists(p, priority(p)); }

			operator std::string() {
				if (L == NULL && R == NULL) return std::to_string(key);

				std::string s = std::to_string(key) + "(";
				s += ((L != NULL) ? (std::string)*L : "NULL") + " ";
				s += ((R != NULL) ? (std::string)*R : "NULL") + ")";
				return s;
			}
		};
		Node *tree;

	public:

		Treap(std::vector<paraula>& v) : Diccionari() { init(v); }
		Treap(const std::vector<paraula>& v) { std::vector<paraula> n = v; init(n); }
		~Treap() { if (tree != NULL) delete tree; }

		operator std::string() const { return (std::string)*tree; }

		bool existeix(paraula p) const { return tree->exists(p); }

		bool optimitza_lot() const { return false; }
		bool optimitza_lot_ordenat() const { return false; }

	};
}

#endif
