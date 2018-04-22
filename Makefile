flags = -std=c++1y
include = -Iutils -Idiccionari

all: test

test: tests/tests.cpp diccionari/diccionari.hpp diccionari/generador.hpp diccionari/paraula.hpp utils/cronometre.hpp utils/display_tabular.hpp utils/file_io.hpp utils/string_cast.hpp
	g++ ${flags} ${include} -o test tests/tests.cpp

check: check_diccionari/diccionari.hpp check_diccionari/generador.hpp check_diccionari/paraula.hpp check_utils/cronometre.hpp check_utils/display_tabular.hpp check_utils/file_io.hpp check_utils/string_cast.hpp

check_%: %.hpp
	g++ ${flags} ${include} -c -o $@ $^
	rm $@