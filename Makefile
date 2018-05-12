flags = -std=c++1y -I./ -O2
diccionari = \
	diccionari/paraula.hpp \
	diccionari/diccionari.hpp \
	diccionari/factory.hpp \
	diccionari/generador.hpp \
	diccionari/MurmurHash3.hpp \
	diccionari/MurmurHash3.cpp \
	\
	diccionari/contenidors_stl.hpp \
	diccionari/filtres.hpp \
	diccionari/search_tree.hpp \
	diccionari/taules_hash.hpp \
	
utils = \
	utils/cronometre.hpp \
	utils/display_tabular.hpp \
	utils/file_io.hpp \
	utils/string_cast.hpp

all: test

test: tests/tests.cpp ${diccionari} ${utils} 
	g++ ${flags} tests/tests.cpp diccionari/MurmurHash3.cpp -o test 

check: check/d_paraula check/d_diccionari check/d_factory check/d_generador check/u_cronometre check/u_display_tabular check/u_file_io check/u_string_cast

check/d_%: diccionari/%.hpp
	g++ ${flags} -c -o $@ $^
	
check/u_%: utils/%.hpp
	g++ ${flags} -c -o $@ $^

clean:
	rm -rf check/d_* check/u_*

ultraclean: clean
	rm -rf test

