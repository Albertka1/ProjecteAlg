#ifndef arxius_hpp
#define arxius_hpp

#include <fstream>
#include <random>
#include <string>
#include <vector>
#include <regex>
#include "paraula.hpp"

namespace diccionari {
	paraula tamanyMax = 10000000000000000000;
        std::vector<std::string> paraulesDicc;
        
	struct {
	private:
		std::uniform_int_distribution<paraula> dist = std::uniform_int_distribution<paraula>(0, tamanyMax - 1);
		std::default_random_engine gen = std::default_random_engine();

	public:
		void llavor(time_t p) { gen.seed((unsigned)p); }
		paraula genera() { return dist(gen); }
                paraula troba(const std::string& contents) {
                    std::uniform_int_distribution<int> dist_int = std::uniform_int_distribution<int>(0, paraulesDicc.size() - 1);
                    int index = dist_int(gen);
                    std::string paraulaString = paraulesDicc.at(index);
                    paraula p = stoull(paraulaString);
                    return p;
                }
                bool rollTheDice(int n){
                    std::uniform_int_distribution<int> distrib = std::uniform_int_distribution<int>(0, 100);
                    int value = distrib(gen);
                    return (value <= n);
               } 
	} motorAleatori;
        
        std::vector<std::string> genera_llista_paraules(const std::string contents){
            //Dividim totes les paraules del diccionari i escollim una
            static const std::regex re{"\\s+"};
            std::vector<std::string> container{
                std::sregex_token_iterator(contents.begin(), contents.end(), re, -1), 
                std::sregex_token_iterator()
            };
            return container;
        }
	int genera_diccionari(const std::string& filename, int size) {
		std::ofstream fs(filename);

		if (!fs.is_open()) return -1;

		// Generem les paraules del diccionari aleatoriament
		for (int i = 0; i < size; ++i) {
			paraula aleatoria = motorAleatori.genera();
			fs << std::to_string(aleatoria);
			fs << std::endl;
		}

		fs.close();
		return 0;
	}
        
        int genera_text(const std::string& filenameText, const std::string& filenameDicc, int size){
                std::ofstream fs(filenameText);
		if (!fs.is_open()) return -1;
                
                std::ifstream gs(filenameDicc);
                std::string diccContent( (std::istreambuf_iterator<char>(gs) ),
                                         (std::istreambuf_iterator<char>()    ) );
                paraulesDicc = genera_llista_paraules(diccContent);
                
		// Generem les paraules del text amb una certa proporcio de les paraules del diccionari
                const double proportion = 0.5;
                const double minimumDiccWords = size * proportion;
                
                int diccWordsAdded = 0;
		for (int i = 0, remaining = size; i < size; ++i, --remaining) {
                    // Roll the dice, si coincideix que encertem afegim una paraula del diccionari, sino aleatòria
                    int possibility = 50; // possibility% d'acertar
                    bool luck = motorAleatori.rollTheDice(possibility);
                    
                    // Si hem tingut sort O si hem exhaurit totes les paraules i no tenim el mínim del diccionari
                    if (luck or (diccWordsAdded < minimumDiccWords and remaining <= minimumDiccWords)){
                        paraula paraulaDelDiccionari = motorAleatori.troba(diccContent);
                        std::cout << "He afegit: " << paraulaDelDiccionari << std::endl;
                        fs << std::to_string(paraulaDelDiccionari);
			fs << std::endl;
                        ++diccWordsAdded;
                    }
                    else{
                        paraula aleatoria = motorAleatori.genera();
			fs << std::to_string(aleatoria);
			fs << std::endl;
                        if (diccContent.find(aleatoria) != std::string::npos) ++diccWordsAdded; // Si per algun motiu coincideix amb alguna actualitzem
                    }
                    
                    std::cout << "W of dicc added: " << diccWordsAdded << std::endl;
                    std::cout << "Remanining: " << remaining << std::endl;
                    std::cout << "----------------------- " << std::endl;
		}

		fs.close();
		return 0;
        }
        
        

}

#endif
