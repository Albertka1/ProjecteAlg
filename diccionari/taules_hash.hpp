#ifndef diccionari_taules_hash_hpp
#define diccionari_taules_hash_hpp


#include <unordered_set>
#include <vector>

#include "diccionari.hpp"

#include "llibreries/sha.h"
#include "llibreries/filters.h"
#include "llibreries/hex.h"
#include "md5.hpp"
#include "xxHash.hpp"
#include "xxhashv232.hpp"
#include "fnv.hpp"

namespace diccionari {   
    // ------------------------- DEFAULT -------------------------------------//
    class HashTableDefault : public Diccionari {
    private:
        std::unordered_set<paraula> uset;
		
    public:
        HashTableDefault(const std::vector<paraula>& pars) : Diccionari(){
            if (pars.size() == 0) return;
            else uset = std::unordered_set<paraula>(pars.begin(),pars.end());
        }
        
        bool existeix(paraula p) const{
            return uset.find(p) != uset.end();
        }
        
        operator std::string() const { 
            std::string s = ""; 
            for (paraula p : uset)
                s+=std::to_string(p); 
            return s;
        }
		float getLoadFactor() override {
			return uset.load_factor();
		}
    };
    //-------------------------- SHA -------------------------//
    struct SHAHashfunc{
		std::string hashToNumber(std::string s) const{
			std::string number;
			for (char c : s) {
				std::string repr;
				if (c >= '0' && c <= '9') repr = std::to_string(9 - (int)('9'-c));
				else repr = std::to_string(25+10 - (int)('Z' - c));
				//std::cout << c << " quals to " << repr << std::endl;
				number.append(repr);
			}
			
			return number;
		}
		std::string shrinkToll(std::string s) const {
			/* Si volem fer la mitja de n parts
			int parts = 4;
			int charsPerPart = std::ceil(s.length() / parts);
			std::vector<std::string> part = std::vector<std::string>(parts);

			//split the string in 'parts' parts
			for (int i = 0; i < parts-1; ++i){
				part[i] = s.substr(charsPerPart*i, charsPerPart);
			}
			part[parts - 1] = s.substr(s.length()-charsPerPart, charsPerPart);

			//for every part compute its mean
			int mean = 0;
			for (int i = 0; i < parts; ++i){
				mean += stoll(part[i]);
			}

			return std::to_string(mean/parts);*/

			/* Sino simplement retallem deixant un digit de marge*/
			int maxdigits = 20-1; //max ull 18446744073709551615
			return s.substr(0, maxdigits);
		}
        size_t operator()(const paraula& p) const {
           
			// Insertar funció hash SHA-216
            std::string value = std::to_string(p);
            CryptoPP::SHA1 hash;
			std::string digest;

			CryptoPP::StringSource(value, true, new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder(new CryptoPP::StringSink(digest))));
			std::string conversion = diccionari::SHAHashfunc::hashToNumber(digest);
			//std::cout << digest << std::endl << std::endl;
			//std::cout << "conv: " << conversion << std::endl;
			std::string shrink = diccionari::SHAHashfunc::shrinkToll(conversion);
			//std::cout << "shrink: " << shrink << std::endl;
			try {
				//std::cout << "max   " << std::numeric_limits<unsigned long long>::max() << std::endl;

				return stoull(shrink);
			}
			catch (const std::exception& ia) {
				std::cout << "max   \t" << std::numeric_limits<unsigned long long>::max() << std::endl;
				std::cout << "shrink:\t" << shrink << std::endl;
				std::cout << ia.what() << std::endl;
				return stoull(shrink.substr(0, shrink.length() - 1));
			} 
        }

    };
    class HashTableSHA : public Diccionari {
    private:
        std::unordered_set<paraula,SHAHashfunc> uset;
        
    public:
        HashTableSHA(const std::vector<paraula>& pars) : Diccionari(){
            if (pars.size() == 0) return;
            else uset = std::unordered_set<paraula,SHAHashfunc>(pars.begin(),pars.end());
        }
        
        bool existeix(paraula p) const{
            return uset.find(p) != uset.end();
        }
        
        operator std::string() const { 
            std::string s = ""; 
            for (paraula p : uset)
                s+=std::to_string(p); 
            return s;
        }
		float getLoadFactor() override {
			return uset.load_factor();
		}
    };
	//-------------------------- DJB2 -------------------------//
	struct DJB2Hashfunc {
		size_t operator()(const paraula& p) const {
			// Insertar funció hash SHA-216
			unsigned long hash = 5381;
			int c;
			const char* str = std::to_string(p).c_str();
			while (c = *str++)
				hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

			return hash;
		}

	};
	class HashTableDJB2 : public Diccionari {
	private:
		std::unordered_set<paraula, DJB2Hashfunc> uset;

	public:
		HashTableDJB2(const std::vector<paraula>& pars) : Diccionari() {
			if (pars.size() == 0) return;
			else uset = std::unordered_set<paraula, DJB2Hashfunc>(pars.begin(), pars.end());
		}

		bool existeix(paraula p) const {
			return uset.find(p) != uset.end();
		}

		operator std::string() const {
			std::string s = "";
			for (paraula p : uset)
				s += std::to_string(p);
			return s;
		}

		float getLoadFactor() override {
			return uset.load_factor();
		}
	};
	//-------------------------- MD5 -------------------------//
	struct MD5Hashfunc {
		std::string hashToNumber(std::string s) const {
			std::string number;
			for (char c : s) {
				std::string repr;
				if (c >= '0' && c <= '9') repr = std::to_string(9 - (int)('9' - c));
				else repr = std::to_string(25 + 10 - (int)('Z' - c));
				//std::cout << c << " quals to " << repr << std::endl;
				number.append(repr);
			}

			return number;
		}
		std::string shrinkToll(std::string s) const {
			/* Si volem fer la mitja de n parts
			int parts = 4;
			int charsPerPart = std::ceil(s.length() / parts);
			std::vector<std::string> part = std::vector<std::string>(parts);

			//split the string in 'parts' parts
			for (int i = 0; i < parts-1; ++i){
			part[i] = s.substr(charsPerPart*i, charsPerPart);
			}
			part[parts - 1] = s.substr(s.length()-charsPerPart, charsPerPart);

			//for every part compute its mean
			int mean = 0;
			for (int i = 0; i < parts; ++i){
			mean += stoll(part[i]);
			}

			return std::to_string(mean/parts);*/

			/* Sino simplement retallem deixant un digit de marge*/
			int maxdigits = 20 - 1; //max ull 18446744073709551615
			return s.substr(0, maxdigits);
		}
		size_t operator()(const paraula& p) const {
			// Insertar funció hash MD5
			std::string value = std::to_string(p);
			std::string digest = md5(value);

			return stoull(shrinkToll(hashToNumber(digest)));
		}
	};
	class HashTableMD5 : public Diccionari {

	private:
		std::unordered_set<paraula, MD5Hashfunc> uset;

	public:
		HashTableMD5(const std::vector<paraula>& pars) : Diccionari() {
			if (pars.size() == 0) return;
			else uset = std::unordered_set<paraula, MD5Hashfunc>(pars.begin(), pars.end());
		}

		bool existeix(paraula p) const {
			return uset.find(p) != uset.end();
		}

		operator std::string() const {
			std::string s = "";
			for (paraula p : uset)
				s += std::to_string(p);
			return s;
		}
		float getLoadFactor() override {
			return uset.load_factor();
		}
	};
	//-------------------------- xxHash -------------------------//
	struct xxHashHashfunc {
		size_t operator()(const paraula& p) const {
			// Insertar funció hash xxHash
			std::string value = std::to_string(p);
			const void* a = value.c_str();
			xxh::hash_t<32> seed = 0x811C9DC5;
			/*paraula result = xxh::xxhash<64>(a, value.length(), t, xxh::endianness::bigEndian);
			return result;*/
			paraula result2 = XXHash32::hash(a, value.length()*sizeof(std::byte), 0x811C9DC5);
			return result2;
		}

	};
	class HashTablexxHash : public Diccionari {
	private:
		std::unordered_set<paraula, xxHashHashfunc> uset;

	public:
		HashTablexxHash(const std::vector<paraula>& pars) : Diccionari() {
			if (pars.size() == 0) return;
			else uset = std::unordered_set<paraula, xxHashHashfunc>(pars.begin(), pars.end());
		}

		bool existeix(paraula p) const {
			return uset.find(p) != uset.end();
		}

		operator std::string() const {
			std::string s = "";
			for (paraula p : uset)
				s += std::to_string(p);
			return s;
		}
		float getLoadFactor() override {
			return uset.load_factor();
		}
	};
	//-------------------------- FNV1a -------------------------//
	struct FNVHashfunc {
		size_t operator()(const paraula& p) const {
			// Insertar funció hash xxHash
			std::string value = std::to_string(p);
			unsigned int hash = FNV::fnv1a(p);
			return hash;
			//return (hash >> 56) ^ (hash & (((unsigned int)1 << 56) - 1));
		}

	};
	class HashTableFNV : public Diccionari {
	private:
		std::unordered_set<paraula, FNVHashfunc> uset;

	public:
		HashTableFNV(const std::vector<paraula>& pars) : Diccionari() {
			if (pars.size() == 0) return;
			else uset = std::unordered_set<paraula, FNVHashfunc>(pars.begin(), pars.end());
		}

		bool existeix(paraula p) const {
			return uset.find(p) != uset.end();
		}

		operator std::string() const {
			std::string s = "";
			for (paraula p : uset)
				s += std::to_string(p);
			return s;
		}
		float getLoadFactor() override {
			return uset.load_factor();
		}
	};

	//-------------------------- Linear Addressing -------------------------//
	class HashSet: public Diccionari {
	private:
		std::vector<paraula> hashset;
		int i = 1;
		int hashAddr;
		enum HashAddressing {
			LinealProbing=1,
			DoubleHashing
		};
		void insertListLinearProbbing(const std::vector<paraula>& pars) {
			for (paraula p : pars) {
				int SETSIZE = hashset.size();
				size_t key = (hashFuncFNV1a(p)) % SETSIZE;
				if (hashset.at(key) == 0) hashset[key] = p;
				else {
					int probe;
					if (key == SETSIZE-1) probe = 0;
					else probe = key + i;

					bool inserted = false;
					// Iterem per tota la taula
					while (probe != key) {
						if (hashset[probe] == 0) { hashset[probe] = p; inserted = true; break; }
						else {
							++probe;
							if (probe >= SETSIZE) probe = 0;
						}
					}
					// Table is full (if i == 1) or all spaces with i != 1 are full -> add space
					if (!inserted) {
						hashset.push_back(p);
					}
				}
			}
		}

		void insertListDoubleHashing(const std::vector<paraula>& pars) {
			for (paraula p : pars) {
				int SETSIZE = hashset.size();
				size_t key1 = (hashFuncFNV1a(p)) % SETSIZE;
				if (hashset.at(key1) == 0) hashset[key1] = p;
				else {
					size_t key2 = (hashFuncxxHash(p)) % SETSIZE;
					int ii = i, maxIters = 10000;
					
					while (1) {
						int probe = (key1 + ii * key2) % SETSIZE;
						if (hashset[probe] == 0) {
							hashset[probe] = p; break;
						}
						if (ii > maxIters) { hashset.push_back(p); ++SETSIZE;  break; }
						ii += i;
					}
				}
			}
		}

	public:
		HashSet(const std::vector<paraula>& pars, int type) {
			hashset = std::vector<paraula>(pars.size(),0);
			hashAddr = type;
			switch (type) {
			case LinealProbing:	insertListLinearProbbing(pars); break;
			case DoubleHashing:	insertListDoubleHashing(pars);	break;
			default:			insertListLinearProbbing(pars);	break;
			}
		}

		size_t hashFuncFNV1a(paraula& p) const {
			std::string value = std::to_string(p);
			unsigned int hash = FNV::fnv1a(p);
			return hash;
		}
		size_t hashFuncxxHash(paraula& p) const {
			std::string value = std::to_string(p);
			const void* a = value.c_str();
			paraula result2 = XXHash32::hash(a, value.length() * sizeof(std::byte), 0x811C9DC5);
			return result2;
		}
		bool existeix(paraula p) const {
			if (hashAddr == LinealProbing) {
				size_t key = HashSet::hashFuncFNV1a(p) % hashset.size();
				if (hashset[key] == p) return true;
				else if (hashset[key] == 0) return false;

				int probe;
				if (key == hashset.size()-1) probe = 0;
				else probe = key + i;

				bool inserted = false;
				// Iterem per tota la taula
				while (probe != key) {
					if (hashset[probe] == p) return true;
					else if (hashset[probe] == 0) return false;
					else {
						++probe;
						if (probe >= hashset.size()) probe = 0;
					}
				}
				return false;
			}
			
			if (hashAddr == DoubleHashing) {
				int SETSIZE = hashset.size();
				size_t key1 = (hashFuncFNV1a(p)) % SETSIZE;
				if (hashset.at(key1) == p) return true;
				else if (hashset.at(key1) == 0) return false;
				else {
					size_t key2 = (hashFuncxxHash(p)) % SETSIZE;
					int ii = i, maxIters = 10000;

					while (ii <= maxIters) {
						int probe = (key1 + ii * key2) % SETSIZE;
						if (hashset[probe] == p) return true;
						if (hashset[probe] == 0) return false;
						ii += i;
					}
					for (int i = SETSIZE - 1; i >= 0; --i) {
						if (hashset[i] == p) return true;
						if (hashset[i] == 0) return false;
					}
				}
			}

			return false;
		}

		operator std::string() const {
			std::string s = "";
			for (paraula p : hashset)
				s += std::to_string(p);
			return s;
		}
		float getLoadFactor() override {
			std::cout << "\tFinal size: " << hashset.size() << std::endl;
			float count = 0;
			for (int i = 0; i < hashset.size(); ++i) {
				if (hashset[i] != 0) ++count;
			}
			return count / hashset.size();
		}
		


	};
}

#endif
