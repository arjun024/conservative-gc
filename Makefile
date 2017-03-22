UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
FLAGS = -std=c++1y -g -O0 -fPIC -D_REENTRANT=1 -fno-omit-frame-pointer # -fsanitize=address -fno-common 

all:
	g++ $(FLAGS) -c gnuwrapper.cpp
	g++ $(FLAGS) -c driver.cpp
	g++ $(FLAGS) -shared gnuwrapper.o driver.o -Bsymbolic -o libgcmalloc.so -ldl -lpthread
	g++ -std=c++1y -g testme.cpp -L. -lgcmalloc -o testme
endif

ifeq ($(UNAME_S),Darwin)
FLAGS = -std=c++14 -g -O0 -fPIC -D_REENTRANT=1 -fno-omit-frame-pointer # -fsanitize=address -fno-common 

all:
	clang++ $(FLAGS) -c macwrapper.cpp
	clang++ $(FLAGS) -c driver.cpp
	# clang++ $(FLAGS) driver.cpp testme.cpp -o testme
	clang++ $(FLAGS) -compatibility_version 1 -current_version 1 -dynamiclib -install_name './libgcmalloc.dylib' macwrapper.o driver.o -o libgcmalloc.dylib
	clang++ -std=c++14 -g testme.cpp -L. -lgcmalloc -o testme
endif
