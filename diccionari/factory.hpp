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
		tSimpleBloom, tCountingBloom, tQuotient, // filtres
		tBST, tTreap, // search_tree
		tMurmur, tSHA, tDJB2, tMD5, txxHash, tFNV, tLinearProbbing, tSeparateChaining // taules_hash
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
		case tSimpleBloom: 		d = new SimpleBloom(pars, args ...); break;
		case tCountingBloom: 	d = new CountingBloom(pars, args ...); break;
		case tQuotient:			d = new Quotient(pars, args ...); break;
		
		// search_tree
		case  tBST:				d = new BinarySearchTree(pars, args ...); break;
		case  tTreap:			d = new Treap(pars, args ...); break;
		
		// taules_hash (1: Linear Probing, 2: Double Hashing)
		case tMurmur:			d = new HashTableDefault(pars, args ...); break;
		case tSHA:				d = new HashTableSHA(pars, args ...); break;
		case tDJB2:				d = new HashTableDJB2(pars, args ...); break;
		case tMD5:				d = new HashTableMD5(pars, args ...); break;
		case txxHash:			d = new HashTablexxHash(pars, args ...); break;
		case tFNV:				d = new HashTableFNV(pars, args ...); break;
		case tLinearProbbing:	d = new HashSet(pars, args ...); break;
		case tSeparateChaining:	d = new HashSetSeparateChaining(pars, args ...); break;

		default:				d = NULL;
		}
                
		return d;
	}
}

#endif
