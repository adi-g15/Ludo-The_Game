B1 = build/object_files
SRC = src

LIBS_OBJECT_FILES = $(B1)/game.o \
                    $(B1)/ludo_box.o \
					$(B1)/ludo_goti.o \
					$(B1)/ludo_coords.o

UTIL_OBJECT_FILES = $(B1)/exceptions.o

LIBS_CPP_FILES = $(SRC)/game.cpp \
                    $(SRC)/ludo_box.cpp \
					$(SRC)/ludo_goti.cpp \
					$(SRC)/ludo_coords.cpp

UTIL_CPP_FILES = $(SRC)/exceptions.cpp

OBJECT_FILES = $(UTIL_OBJECT_FILES) \
			   $(LIBS_OBJECT_FILES) \
               build/main.o

 #if mode vaiable is empty, setting mode to debug
ifeq ($(mode),release)
   CXXFLAGS = -Wall
else
   mode = debug
   CXXFLAGS = -g -Wall
endif

#FUTURE - Later shift to dynamic and static libraries as needed, for now just do static linking
build/game : $(OBJECT_FILES) create_static_lib create_utilities_lib
	g++ build/main.o -Llibs -lludo_static -lutil -o build/game

build/debug : $(UTIL_OBJECT_FILES) $(LIBS_OBJECT_FILES) create_static_lib create_utilities_lib
	g++ main.cpp -Llibs -lludo_static -lutil -Iincludes -g -o build/debug

build/main.o: main.cpp create_build_directories
	g++ -c main.cpp $(CXXFLAGS) -Iincludes -o build/main.o

##LIBRARY_ OBJECT_ FILES START  (Compiling implementation files)
$(B1)/game.o : $(SRC)/game.cpp
	g++ -c $(SRC)/game.cpp $(CXXFLAGS) -std=c++17 -Iincludes -o $(B1)/game.o

$(B1)/exceptions.o : create_build_directories $(SRC)/exceptions.cpp
	g++ -c $(SRC)/exceptions.cpp $(CXXFLAGS) -Iincludes -o $(B1)/exceptions.o

$(B1)/ludo_coords.o : $(SRC)/ludo_coords.cpp
	g++ -c $(SRC)/ludo_coords.cpp $(CXXFLAGS) -Iincludes -o $(B1)/ludo_coords.o

$(B1)/ludo_box.o : $(SRC)/ludo_box.cpp
	g++ -c $(SRC)/ludo_box.cpp $(CXXFLAGS) -Iincludes -o $(B1)/ludo_box.o

$(B1)/ludo_goti.o : $(SRC)/ludo_goti.cpp
	g++ -c $(SRC)/ludo_goti.cpp $(CXXFLAGS) -Iincludes -o $(B1)/ludo_goti.o
##LIBRARY_ OBJECT_ FILES END

##Creating Libraries START
create_utilities_lib : $(UTIL_OBJECT_FILES) $(UTIL_CPP_FILES)
	ar rvs libs/libutil.a $(UTIL_OBJECT_FILES)

create_static_lib : $(LIBS_OBJECT_FILES) $(LIBS_CPP_FILES)
	ar rvs libs/libludo_static.a $(LIBS_OBJECT_FILES)

create_dynamic_lib : $(LIBS_OBJECT_FILES) $(LIBS_OBJECT_FILES)
	echo "TODO"

##Creating Libraries END

clean:
	rm build/ libs/ -rvf

run: build/game
	./build/game

debug: build/debug
	gdb ./build/debug

#   'UTILITY'
create_build_directories: 
	mkdir build/object_files -p
	mkdir libs -p

exp: 
	g++ experimental.cpp -o 'build/exp'
	build/exp
#   'UTILITY'
