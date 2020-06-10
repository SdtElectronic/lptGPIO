demo: demo.cxx parIO.cxx
	clang -o demo demo.cxx parIO.cxx -lstdc++ -lpthread -O2
