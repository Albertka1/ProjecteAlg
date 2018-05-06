#ifndef diccionari_filtres_hpp
#define diccionari_filtres_hpp

#include <vector>
#include <functional>
#include <iterator>
#include <set>
#include <unordered_set>
#include <stdint.h>
//#include <stdio.h>
#include <string.h>

//#include <sstream>

#include <iostream>
#include <iomanip>

#include "diccionari.hpp"
#include "MurmurHash3.h"
using namespace std;

namespace diccionari {
	class SimpleBloom : public Diccionari {
		private:
			std::vector<paraula> pars;
			std::vector<bool> bits;
			//int n;  //tamany del vector pars
			unsigned int k= 3;   //numero de hash per paraula
			unsigned int m;   //tamany del vector bits
			
			void setFalses() { bits = std::vector<bool>(m,false); }
				
			unsigned int Stupid_hash(int k, paraula p) const {
				unsigned int sum=0;
				while (p > 0) {
					sum += p%10;
					p /=10;
				}
				sum = sum<<k-1;
				return sum % m;                          
			}
				
				
			unsigned long Murmur_hash(int k, paraula p) const {
				uint64_t seed = (uint64_t)k;
				uint64_t *hash_otpt[2];
				
				//cout << "p: "<< p << endl;
				
				const unsigned long long *key = &p;
				
				//const unsigned char *key2 = (unsigned char*) &p;
								
				//cout << "key: " << *key << endl;
				//cout << "key2: " << *key2 << endl;				
				//void MurmurHash3_x86_32  ( const void * key, int len, uint32_t seed, void * out );
				//void MurmurHash3_x64_128 ( const void * key, int len, uint32_t seed, void * out );
				MurmurHash3_x64_128(key, sizeof(paraula), seed, hash_otpt);
				//MurmurHash3_x86_32(key2, sizeof(unsigned char), seed, hash_otpt);
				//cout << "P[value]: " << p << endl;
				
				unsigned long long int res;
				stringstream ss;
				ss << hash_otpt[0];
				ss >> hex >>res;
				
				//cout << "hashed(hex) " << hash_otpt[0] << endl;
				//cout << "hashed(dec) " << res << endl;
				//cout << "final hash: " << res%m << endl;
				//cout << endl;		
								
				return (unsigned long)res%m;
			}
				
			void Simple(const std::vector<paraula>& v, int k) {
				int kk = k<<2;
				for (paraula p: v) {
					unsigned long hash = Murmur_hash(kk,p);
					cout << "paraula : " << p << ", hash: " << hash << endl; 
				 	bits[hash] = true;
				}
			}
		
		public:
			SimpleBloom (const std::vector<paraula>& v) : Diccionari() { 
				pars = std::vector<paraula>(v);
				m = 2*k*v.size();   //m > k*n
				cout << "el numero N es: " << v.size() << endl << endl;
				cout << "el numero M es: " << m << endl << endl;
				setFalses();
				for (int i = 0; i < k; i++) Simple(v, i);
			}
			
			macro_stringCast
			bool existeix(paraula p) const { 
				bool conte = true;
				for (int i = 0; i < k; i++){
					//int sh = Stupid_hash(k,p);
					//if (bits[p] == false) conte = false;
					int ii = i<<2;
					if (bits[Murmur_hash(ii,p)] == false) conte = false;
				}
				return conte; 
			}
			
	};

	class CountingBloom : public Diccionari {
		private:
			std::vector<paraula> pars;
            std::vector<uint8_t> counters; //valores [0,255]

            int k=1;   //numero de hash per paraula
            int m;   //tamany del vector bits
                        
            uint8_t Stupid_hash(int k, paraula p) const {
				uint8_t sum=0
				while (p > 0) {
					sum += p%10;
					p /=10;
				}
				sum = sum<<k-1;
				return sum % m; 
            }
            
            void setZeros() { counters = std::vector<bool>(m, 0); }
            
            unsigned long Murmur_hash(int k, paraula p) const {
				uint64_t seed = (uint64_t)k;
				uint64_t *hash_otpt[2];
				const unsigned long long *key = &p;
				
				//void MurmurHash3_x86_32  ( const void * key, int len, uint32_t seed, void * out );
				//void MurmurHash3_x64_128 ( const void * key, int len, uint32_t seed, void * out );
				MurmurHash3_x64_128(key, sizeof(paraula), seed, hash_otpt);
				//MurmurHash3_x86_32(key2, sizeof(unsigned char), seed, hash_otpt);
				
				unsigned long long int res;
				stringstream ss;
				ss << hash_otpt[0];
				ss >> hex >>res;
								
				return (unsigned long)res%m;
			}
                        
            void Counting(const std::vector<paraula>& v, const int k) {
				int kk = k<<2;
				for (paraula p: v) {
					unsigned long hash = Murmur_hash(kk,p);
					counters[hash]++;
                }
            }
		public:
			CountingBloom(const std::vector<paraula>& v) : Diccionari() { 
				pars = std::vector<paraula>(v); 
                setZeros();
                for (int i = 0; i < k; i++) Couting(v, i); 
            }
			
			macro_stringCast
			bool existeix(paraula p) const{
				bool conte = true;
                for (int i = 0; i < k; i++){
					int ii = i<<2;
					if (counters[Murmur_hash(ii,p)] == 0) conte = false; 
                }
                return conte;
            }
                        
            bool eliminar(paraula p) {
				for (int i = 0; i < k; i++) {
					int ii = i<<2;
					counters[Murmur_hash(ii,p)]--;
				}
			}
			
	};
	
	class Quotient : public Diccionari {
		private:
		
		public:
			Quotient(const std::vector<paraula>& v) : Diccionari() {
				//cosas Peter
			}
			
			macro_stringCast
			bool existeix(paraula p) const{
				bool conte = true;

                return conte;
            }

}

#endif
