#ifndef diccionari_factory_hpp
#define diccionari_factory_hpp

#include "diccionari.hpp"
#include "search_tree.hpp"

namespace diccionari {
	enum DictType {tCercaSequencial = 1, tSetFind, tUSetFind, tBTree, tBinarySearchTree, tTreap};

	Diccionari* factory(int type, const std::vector<paraula>& pars) {
		Diccionari* d;
		switch (type) {
		case  1: d = new CercaSequencial(pars); break;
		case  2: d = new SetFind(pars); break;
		case  3: d = new USetFind(pars); break;
		// case  4: d = new BTree(pars); break;
		case  5: d = new BinarySearchTree(pars); break;
		// case  6: new Treap(pars); break;
		default: d = NULL;
		}

		return d;
	}
}

#endif