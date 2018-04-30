#ifndef diccionari_factory_hpp
#define diccionari_factory_hpp

#include <iostream>

#include "diccionari.hpp"
#include "contenidors_stl.hpp"
#include "filtres.hpp"
#include "search_tree.hpp"
#include "taules_hash.hpp"

namespace diccionari {
	enum DictType {
		tCercaSequencial=1, tSetFind, tUSetFind, // contenidors_stl
		// filtres
		tBST, tTreap, // search_tree
		// taules_hash
         Murmur, SHA, DJB2, MD5, xxHash, FNV
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
		case  tBST: d = new BinarySearchTree(pars); break;
		// case  tTreap: new Treap(pars); break;
		
		// taules_hash
		case Murmur:	d = new HashTableDefault(pars);	std::cout << "Murmur" << std::endl;						break;
		case SHA:		d = new HashTableSHA(pars);		std::cout << "SHA amb shrink per tall" << std::endl;	break;
		case DJB2:		d = new HashTableDJB2(pars);	std::cout << "DJB2" << std::endl;						break;
		case MD5:		d = new HashTableMD5(pars);		std::cout << "MD5" << std::endl;						break;
		case xxHash:	d = new HashTablexxHash(pars);	std::cout << "xxHash" << std::endl;						break;
		case FNV:		d = new HashTableFNV(pars);		std::cout << "FNV" << std::endl;						break;
		default: d = NULL;
		}
                
		return d;
	}
}

#endif
