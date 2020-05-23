B1 = build/object_files
I1 = includes

LIBS_OBJECT_FILES = $(B1)/board.o \
                    $(B1)/ludo_box.o \
					$(B1)/ludo_goti.o

UTIL_OBJECT_FILES = $(B1)/exceptions.o

LIBS_CPP_FILES = $(I1)/board.cpp \
                    $(I1)/ludo_box.cpp \
					$(I1)/ludo_goti.cpp

UTIL_CPP_FILES = $(I1)/exceptions.cpp

OBJECT_FILES = $(UTIL_OBJECT_FILES) \
			   $(LIBS_OBJECT_FILES) \
               build/main.o

#FUTURE - Later shift to dynamic and static libraries as needed, for now just do static linking
build/game : $(OBJECT_FILES) create_static_lib create_utilities_lib
	g++ build/main.o -Llibs -lutil -lludo_static -o build/game

build/game_debug : $(OBJECT_FILES) create_static_lib create_utilities_lib
	g++ build/main.o -Llibs -lutil -lludo_static -g -o build/game_debug

build/main.o: main.cpp
	g++ -c main.cpp -Iincludes -o build/main.o

##LIBRARY_ OBJECT_ FILES START  (Compiling implementation files)
$(B1)/board.o : $(I1)/board.cpp
	g++ -c $(I1)/board.cpp -Iincludes -o $(B1)/board.o

$(B1)/exceptions.o : $(I1)/exceptions.cpp
	g++ -c $(I1)/exceptions.cpp -Iincludes -o $(B1)/exceptions.o

$(B1)/ludo_box.o : $(I1)/ludo_box.cpp
	g++ -c $(I1)/ludo_box.cpp -Iincludes -o $(B1)/ludo_box.o

$(B1)/ludo_goti.o : $(I1)/ludo_goti.cpp
	g++ -c $(I1)/ludo_goti.cpp -Iincludes -o $(B1)/ludo_goti.o
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
	rm build/main.o build/object_files/*.o build/game*

run: build/game
	./build/game

debug: build/game_debug
	gdb ./build/game_debug
