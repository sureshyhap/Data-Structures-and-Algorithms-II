spellcheck: spellcheck.o hash.o
	g++ -o spellcheck spellcheck.o hash.o
spellcheck.o: spellcheck.cpp hash.h
	g++ -c spellcheck.cpp
hash.o: hash.cpp hash.h
	g++ -c hash.cpp
