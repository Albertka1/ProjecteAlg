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
#include <cmath>

//#include <sstream>

#include <iostream>
#include <iomanip>

#include "diccionari.hpp"
#include "MurmurHash3.hpp"
using namespace std;

namespace diccionari {
	class SimpleBloom : public Diccionari { //rapido, no permite deletes, coste espacial bajo
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
				
				
			unsigned long long Murmur_hash(int k, paraula p) const {
				//cout << endl << "MURMURHASH" << endl;
				uint32_t seed = (uint32_t)k;
				uint32_t *hash_otpt[2] = {0};
				
				//cout << "p: "<< p << endl;
				
				const unsigned long long *key = &p;
				
				//const char *key2 = "2";
								
				//cout << "key: " << *key << endl;
				//cout << "key2: " << *key2 << endl;				
				//void MurmurHash3_x86_32  ( const void * key, int len, uint32_t seed, void * out );
				//void MurmurHash3_x64_128 ( const void * key, int len, uint32_t seed, void * out );
				MurmurHash3_x86_32(key, sizeof(paraula), seed, hash_otpt);
				//MurmurHash3_x86_32(key2, (uint64_t)strlen(key2), seed, hash_otpt);
				//cout << "P[value]: " << p << endl;
				
				unsigned long long res;
				stringstream ss;
				ss << *hash_otpt;
				ss >> hex >>res;
				
				//cout << "hashed(hex or) " << *hash_otpt << endl;
				//cout << "hashed(hex) " << hash_otpt[0] << endl;
				//cout << "hashed(dec) " << res << endl;
				//cout << "final hash: " << res%m << endl;
				//cout << endl;		
								
				return res%m;
			}
				
			void Simple(const std::vector<paraula>& v, int k) {
				//unsigned long long hash = Murmur_hash(1, v[0]);
				//cout << "paraula : " << v[0] << ", hash: " << hash << endl;
				unsigned int kk = k<<2;
				for (paraula p: v) {
					//unsigned long long hash = Murmur_hash(kk,p);
					//cout << "paraula : " << p << ", seed:"<< kk << ", hash: " << hash << endl; 
				 	bits[Murmur_hash(kk,p)] = true;
				}
			}
		
		public:
			SimpleBloom (const std::vector<paraula>& v) : Diccionari() { 
				pars = std::vector<paraula>(v);
				m = 4*k*v.size();   //m > k*n
				//cout << "el numero N es: " << v.size() << endl << endl;
				//cout << "el numero M es: " << m << endl << endl;
				setFalses();
				for (int i = 0; i < k; i++) Simple(v, i);
			}
			
			macro_stringCast
			bool existeix(paraula p) const { 
				bool conte_bool = true;
				//cout << endl;
				for (int i = 0; i < k; i++){
					unsigned int ii = i<<2;
					//unsigned long long hash = Murmur_hash(ii,p);
					//cout << "paraula : " << p << ", seed:"<< ii << ", hash: " << hash << endl; 
					if (bits[Murmur_hash(ii,p)] == false) conte_bool = false;
				}
				return conte_bool; 
			}
			
	};

	class CountingBloom : public Diccionari { //rapido, permite deletes, coste espacial alto
		private:
			std::vector<paraula> pars;
            std::vector<uint8_t> counters; //valores [0,255]

            int k=1;   //numero de hash per paraula
            int m;   //tamany del vector bits
                        
            uint8_t Stupid_hash(int k, paraula p) const {
				uint8_t sum=0;
				while (p > 0) {
					sum += p%10;
					p /=10;
				}
				sum = sum<<k-1;
				return sum % m; 
            }
            
            void setZeros() { counters = std::vector<uint8_t>(m, 0); }
            
            unsigned long long Murmur_hash(int k, paraula p) const {
				uint32_t seed = (uint32_t)k;
				uint32_t *hash_otpt[2] = {0};
				const unsigned long long *key = &p;
				
				//void MurmurHash3_x86_32  ( const void * key, int len, uint32_t seed, void * out );
				//void MurmurHash3_x64_128 ( const void * key, int len, uint32_t seed, void * out );
				MurmurHash3_x86_32(key, sizeof(paraula), seed, hash_otpt);
				//MurmurHash3_x86_32(key2, sizeof(unsigned char), seed, hash_otpt);
				
				unsigned long long res;
				stringstream ss;
				ss << *hash_otpt;
				ss >> hex >>res;
								
				return res%m;
			}
                        
            void Counting(const std::vector<paraula>& v, const int k) {
				int kk = k<<2;
				for (paraula p: v) {
					counters[Murmur_hash(kk,p)]++;
                }
            }
		public:
			CountingBloom(const std::vector<paraula>& v) : Diccionari() { 
				pars = std::vector<paraula>(v); 
                setZeros();
                for (int i = 0; i < k; i++) Counting(v, i); 
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
	
	class Quotient : public Diccionari { //deberia ser lento (permite deletes, coste espacial bajo)
		private:
			struct quotient_slot {
				quotient_slot(): fr(0), is_occupied(false), is_continuation(false), is_shifted(false){}
				unsigned long fr; //remainder
				bool is_occupied;
				bool is_continuation;
				bool is_shifted;
			};
			
			std::vector<paraula> pars;
			std::vector<fr> quotient;
			unsigned int m;   //tamany del hash table
			unsigned int p = 8;
			 
			//usare murmurhash (unicamente)
			
			//we need a fingersprint f
			//p-bit fingersprint  (qf stores p-bit of element)
			//r least significant bits (remainder)
			//q = p - r most significant bits (quotient)
			//hash table has 2^q slots
			
			//fr = f mod 2^r  (remainder)
			//fq = |_ f/2^r_| (quotient)
			//remainder is stored in the bucket indexed by the quotient
			//each bucket contains 3 bits, all 0 at th begining
			//is_occupied, is_continuation, is_shifted
			
			unsigned long Murmur_hash(paraula p) const {
				uint32_t seed = 95;
				uint32_t *hash_otpt[2] = {0};
				const unsigned long long *key = &p;

				MurmurHash3_x86_32(key, sizeof(paraula), seed, hash_otpt);
				
				unsigned long long res;
				stringstream ss;
				ss << *hash_otpt;
				ss >> hex >>res;
				
				return (unsigned long)res%m;
			}
			
			
			void add_element(const unsigned long f) {
				int mod = pow(2.0, r)
				int remainder = f % mod; //fr
				int qindex = floor(f / mod); //fq
				
				quotient_slot canonical = quotient[qindex];
				
				if (!canonical.is_occupied and !canonical.is_continuation and !canonical.is_shifted) { //0;0;0;
					quotient[qindex].is_occupied = true;
					quotient[qindex].fr = remainder;
				}
				else if (!canonical.is_occupied and canonical.is_continuation and canonical.is_shifted) { //0;1;1;
					quotient[qindex].is_occupied = true;
					quotient[qindex+1].is_shifted = true;
					quotient[qindex+1].fr = remainder;
				}
				
				else if (canonical.is_occupied and !quotient[qindex].is_continuation and !quotient[qindex].is_shifted) { //1;0;0;
					if (canonical.fr < remainder) {
						quotient[qindex+1].is_continuation = true;
						quotient[qindex+1].is_
					}
					else if (canonical.fr >= remainder) {
						//cosas
					}
				}
				else if (!)
			}
			
			
		public:
			Quotient(const std::vector<paraula>& v) : Diccionari() {
				unsigned int r = 4;
				unsigned int q = p-r;
				pars = std::vector<paraula>(v);
				m = (unsigned int)pow(2.0,q);
				for (paraula p: v) {
					unsigned long f = Murmur_hash(p);
					add_element(f);
				}
				
			}
			
			macro_stringCast
			bool existeix(paraula p) const{
				bool conte = true;
                return conte;
            }
	};

}

#endif
