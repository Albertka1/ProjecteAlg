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

			void insert(paraula p, long long int *NC) {
				if (NC != NULL) ++(*NC);
				if (p < key) {
					if (L != NULL) L->insert(p, NC);
					else L = new Node(p);
				}
				else {
					if (NC != NULL) ++(*NC);
					if (p > key) {
						if (R != NULL) R->insert(p, NC);
						else R = new Node(p);
					}
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

			bool exists(paraula p, long long int *NC) {
				if (NC != NULL) ++(*NC);
				if (p < key && L != NULL) return L->exists(p, NC);

				if (NC != NULL) ++(*NC);
				if (p > key && R != NULL) return R->exists(p, NC);

				if (NC != NULL) ++(*NC);
				return (p == key);
			}

			// Returns X where
			// lot[X-1] < key <= lot[X]
			// a <= X
			// X <= b+1
			unsigned split(const std::vector<paraula>& lot, unsigned a, unsigned b, long long int *NC) {
				unsigned pivot;
				while (a < b) {
					pivot = (a + b) / 2;
					if (NC != NULL) ++(*NC);
					if (lot[pivot] < key)
						a = pivot + 1;
					else
						b = pivot;
				}

				return a;
			}

			void set_search(const std::vector<paraula>& lot, std::vector<bool>& trobats, unsigned a, unsigned b, long long int *NC) {
				unsigned pivot = split(lot, a, b, NC);
				if (NC != NULL) ++(*NC);
				if (L != NULL)
					L->set_search(lot, trobats, a, pivot, NC);
				if (pivot < lot.size()) {
					if (lot[pivot] == key) {
						trobats[pivot] = true;
						if (R != NULL)
							R->set_search(lot, trobats, pivot + 1, b, NC);
					}
					else if (R != NULL)
						R->set_search(lot, trobats, pivot, b, NC);
				}
			}

			std::vector<bool> q_search(std::vector<paraula>& lot, long long int *NC) {
				bool hit = false;
				std::vector<bool> trobats = std::vector<bool>();
				std::vector<paraula> vLeft = std::vector<paraula>();
				std::vector<paraula> vRight = std::vector<paraula>();

				for (unsigned i = 0; i < lot.size(); ++i) {
					if (NC != NULL) ++(*NC);
					if (lot[i] < key) vLeft.push_back(lot[i]);
					else {
						if (NC != NULL) ++(*NC);
						if (lot[i] > key) vRight.push_back(lot[i]);
						else hit = true;
					}
				}

				if (L != NULL) trobats = L->q_search(vLeft, NC);
				else {
					trobats = std::vector<bool>(vLeft.size(), false);
					sort(vLeft.begin(), vLeft.end());
				}

				if (hit) trobats.push_back(true);

				std::vector<bool> tmp;
				if (R != NULL) tmp = R->q_search(vRight, NC);
				else {
					tmp = std::vector<bool>(vRight.size(), false);
					sort(vRight.begin(), vRight.end());
				}
				trobats.insert(trobats.end(), tmp.begin(), tmp.end());

				lot = vLeft;
				if (hit) lot.push_back(key);
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
		mutable long long int n_comp = 0;

	public:
		BinarySearchTree(const std::vector<paraula>& v) : Diccionari() {
			if (v.size() == 0) tree = NULL;
			else {
				tree = new Node(v[0]);
				for (unsigned i = 1; i < v.size(); ++i) {
					tree->insert(v[i], &n_comp);
				}
				tree = tree->DSW(v.size());
			}
		}
		~BinarySearchTree() { if (tree != NULL) delete tree; }

		operator std::string() const { return (std::string)*tree; }

		bool existeix(paraula p) const {
			if (tree == NULL) return false;
			return tree->exists(p, &n_comp);
		}

		bool optimitza_lot() const { return true; }
		std::vector<bool> existeix_lot(std::vector<paraula>& lot) const { return tree->q_search(lot, &n_comp); }

		bool optimitza_lot_ordenat() const { return true; }
		std::vector<bool> existeix_lot_ordenat(const std::vector<paraula>& lot) const {
			std::vector<bool> trobats = std::vector<bool>(lot.size(), false);
			tree->set_search(lot, trobats, 0, lot.size()-1, &n_comp);
			return trobats;
		}
		
		long long int count_comps() const { return n_comp; }
		void restart_count() { n_comp = 0; }
	};

	class Treap : public Diccionari {
	private:
		typedef int prio_type;
		static prio_type priority(paraula p) { return prio_type(p % (PARAULA_MAX/100)); }
		static bool comp_p(paraula a, paraula b) { return priority(a) > priority(b); }

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

			void insert(paraula p, long long int *NC) {
				if (NC != NULL) ++(*NC);
				if (p < key) {
					if (L != NULL) L->insert(p, NC);
					else L = new Node(p);
				}
				else {
					if (NC != NULL) ++(*NC);
					if (p > key) {
						if (R != NULL) R->insert(p, NC);
						else R = new Node(p);
					}
				}
			}

			bool exists(paraula p, prio_type pr, long long int *NC) {
				if (pr > prio) return false;

				if (NC != NULL) ++(*NC);
				if (p < key) {
					if (L != NULL)
						return L->exists(p, pr, NC);
					return false;
				}

				if (NC != NULL) ++(*NC);
				if (p > key) {
					if (R != NULL)
						return R->exists(p, pr, NC);
					return false;
				}

				return true;
			}
			bool exists(paraula p, long long int *NC) { return exists(p, priority(p), NC); }

			// Returns X where
			// lot[X-1] < key <= lot[X]
			// a <= X
			// X <= b+1
			unsigned split(const std::vector<paraula>& lot, unsigned a, unsigned b, long long int *NC) {
				unsigned pivot;
				while (a < b) {
					pivot = (a + b) / 2;
					if (NC != NULL) ++(*NC);
					if (lot[pivot] < key)
						a = pivot + 1;
					else
						b = pivot;
				}

				return a;
			}
			
			void set_search(const std::vector<paraula>& lot, std::vector<bool>& trobats, unsigned a, unsigned b, long long int *NC) {
				unsigned pivot = split(lot, a, b, NC);
				if (NC != NULL) ++(*NC);
				if (L != NULL)
					L->set_search(lot, trobats, a, pivot, NC);
				if (pivot < lot.size()) {
					if (lot[pivot] == key) {
						trobats[pivot] = true;
						if (R != NULL)
							R->set_search(lot, trobats, pivot + 1, b, NC);
					}
					else if (R != NULL)
						R->set_search(lot, trobats, pivot, b, NC);
				}
			}

			std::vector<bool> q_search(std::vector<paraula>& lot, long long int *NC) {
				bool hit = false;
				std::vector<bool> trobats = std::vector<bool>();
				std::vector<paraula> vLeft = std::vector<paraula>();
				std::vector<paraula> vRight = std::vector<paraula>();

				for (unsigned i = 0; i < lot.size(); ++i) {
					if (NC != NULL) ++(*NC);
					if (lot[i] < key) vLeft.push_back(lot[i]);
					else {
						if (NC != NULL) ++(*NC);
						if (lot[i] > key) vRight.push_back(lot[i]);
						else hit = true;
					}
				}

				if (L != NULL) trobats = L->q_search(vLeft, NC);
				else {
					trobats = std::vector<bool>(vLeft.size(), false);
					sort(vLeft.begin(), vLeft.end());
				}

				if (hit) trobats.push_back(true);

				std::vector<bool> tmp;
				if (R != NULL) tmp = R->q_search(vRight, NC);
				else {
					tmp = std::vector<bool>(vRight.size(), false);
					sort(vRight.begin(), vRight.end());
				}
				trobats.insert(trobats.end(), tmp.begin(), tmp.end());

				lot = vLeft;
				if (hit) lot.push_back(key);
				lot.insert(lot.end(), vRight.begin(), vRight.end());

				return trobats;
			}

			operator std::string() {
				std::string s = std::to_string(key) + ":" + std::to_string(prio);
				if (!(L == NULL && R == NULL)) {
					s += "(";
					s += ((L != NULL) ? (std::string)*L : "NULL") + " ";
					s += ((R != NULL) ? (std::string)*R : "NULL") + ")";
				}
				return s;
			}
		};

		void init(std::vector<paraula>& v) {
			if (v.size() == 0) tree = NULL;
			else {
				sort(v.begin(), v.end(), comp_p);
				tree = new Node(v[0]);
				for (unsigned i = 1; i < v.size(); ++i) {
					tree->insert(v[i], &n_comp);
				}
			}
		}

		Node *tree;
		mutable long long int n_comp = 0;

	public:

		Treap(std::vector<paraula>& v) : Diccionari() { init(v); }
		Treap(const std::vector<paraula>& v) { std::vector<paraula> n = v; init(n); }
		~Treap() { if (tree != NULL) delete tree; }

		operator std::string() const { return (std::string)*tree; }

		bool existeix(paraula p) const {
			if (tree == NULL) return false;
			return tree->exists(p, &n_comp);
		}

		bool optimitza_lot() const { return true; }
		std::vector<bool> existeix_lot(std::vector<paraula>& lot) const { return tree->q_search(lot, &n_comp); }

		bool optimitza_lot_ordenat() const { return true; }
		std::vector<bool> existeix_lot_ordenat(const std::vector<paraula>& lot) const {
			std::vector<bool> trobats = std::vector<bool>(lot.size(), false);
			tree->set_search(lot, trobats, 0, lot.size() - 1, &n_comp);
			return trobats;
		}

		long long int count_comps() const { return n_comp; }
		void restart_count() { n_comp = 0; }
	};
}

#endif
