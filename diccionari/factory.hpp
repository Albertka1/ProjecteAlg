#ifndef diccionari_factory_hpp
#define diccionari_factory_hpp

#include "diccionari.hpp"
#include "contenidors_stl.hpp"
#include "filtres.hpp"
#include "search_tree.hpp"
#include "taules_hash.hpp"

namespace diccionari {
	enum DictType {
		tCercaSequencial=1, tSetFind, tUSetFind, // contenidors_stl
		// filtres
		tBTree, tBST, tTreap, // search_tree
		// taules_hash
	};

	Diccionari* factory(int type, const std::vector<paraula>& pars) {
		Diccionari* d;
		switch (type) {
		// contenidors_stl
		case  tCercaSequencial: d = new CercaSequencial(pars); break;
		case  tSetFind: d = new SetFind(pars); break;
		case  tUSetFind: d = new USetFind(pars); break;
		
		// filtres
		
		// search_tree
		// case  tBTree: d = new BTree(pars); break;
		case  tBST: d = new BinarySearchTree(pars); break;
		// case  tTreap: new Treap(pars); break;
		
		// taules_hash
		
		default: d = NULL;
		}

		return d;
	}
}

#endif
