flags = -std=c++1y
include = -Iutils -Idiccionari
diccionari = diccionari/diccionari.hpp diccionari/generador.hpp diccionari/paraula.hpp
utils = utils/cronometre.hpp utils/display_tabular.hpp utils/file_io.hpp utils/string_cast.hpp

all: test

test: tests/tests.cpp ${diccionari} ${utils}
	g++ ${flags} ${include} -o test tests/tests.cpp

check: check/d_diccionari check/d_generador check/d_paraula check/u_cronometre check/u_display_tabular check/u_file_io check/u_string_cast

check/d_%: diccionari/%.hpp
	g++ ${flags} ${include} -c -o $@ $^
	
check/u_%: utils/%.hpp
	g++ ${flags} ${include} -c -o $@ $^

clean:
	rm -rf check/d_* check/u_*

ultraclean: clean
	rm -rf test
