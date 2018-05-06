#ifndef diccionari_factory_hpp
#define diccionari_factory_hpp

#include <iostream>

#include "diccionari.hpp"
#include "contenidors_stl.hpp"
//#include "filtres.hpp"
#include "search_tree.hpp"
#include "taules_hash.hpp"

namespace diccionari {
	enum DictType {
		tCercaSequencial=1, tSetFind, tUSetFind, // contenidors_stl
		// filtres
		tBST, tTreap, // search_tree
		Murmur, SHA, DJB2, MD5, xxHash, FNV, LinearProbbing // taules_hash
	};

	template <class ... Args>
	Diccionari* factory(int type, const std::vector<paraula>& pars, Args ... args) {
		Diccionari* d;
		switch (type) {
		// contenidors_stl
		case  tCercaSequencial:	d = new CercaSequencial(pars, args ...); break;
		case  tSetFind:			d = new SetFind(pars, args ...); break;
		case  tUSetFind:		d = new USetFind(pars, args ...); break;
		
		// filtres
		
		// search_tree
		case  tBST:				d = new BinarySearchTree(pars, args ...); break;
		case  tTreap:			d = new Treap(pars, args ...); break;
		
		// taules_hash (1: Linear Probing, 2: Double Hashing)
		case Murmur:			d = new HashTableDefault(pars, args ...);	std::cout << "Murmur" << std::endl;						break;
		case SHA:				d = new HashTableSHA(pars, args ...);		std::cout << "SHA amb shrink per tall" << std::endl;	break;
		case DJB2:				d = new HashTableDJB2(pars, args ...);	std::cout << "DJB2" << std::endl;						break;
		case MD5:				d = new HashTableMD5(pars, args ...);		std::cout << "MD5" << std::endl;						break;
		case xxHash:			d = new HashTablexxHash(pars, args ...);	std::cout << "xxHash" << std::endl;						break;
		case FNV:				d = new HashTableFNV(pars, args ...);		std::cout << "FNV" << std::endl;						break;
		case LinearProbbing:	d = new HashSet(pars, 2);		std::cout << "Linear probbing" << std::endl;			break;
		default:				d = NULL;
		}
                
		return d;
	}
}

#endif
