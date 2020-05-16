B1 = build/object_files
I1 = includes

#LIBS_OBJECT_FILES = $(B1)/exceptions.o \
#                    $(B1)/util_func.o \
#					$(B1)/simpleTuple.o \
#					$(B1)/ludo_goti.o \
#					$(B1)/ludo_box.o \
#                    $(B1)/board.o
#
#OBJECT_FILES = $(LIBS_OBJECT_FILES) build/main.o

LIBS_OBJECT_FILES = $(B1)/board.o \
                    $(B1)/ludo_box.o \
					$(B1)/ludo_goti.o \
					$(B1)/ludo_coords.o

UTIL_OBJECT_FILES = $(B1)/utilities.o \
					$(B1)/exceptions.o

OBJECT_FILES = $(UTIL_OBJECT_FILES) \
			   $(LIBS_OBJECT_FILES) \
               build/main.o

#FUTURE - Later shift to dynamic and static libraries as needed, for now just do static linking
build/game : $(OBJECT_FILES) create_static_lib create_utilities_lib
#	g++ build/main.o $(B1)/board.o \
#                        $(B1)/ludo_box.o \
#                        $(B1)/ludo_goti.o \
#			-Llibs -lutil -o build/game
	g++ build/main.o -Llibs -lutil -lludo_static -o build/game

build/main.o: main.cpp
	g++ -E main.cpp -Iincludes > main
	g++ -c main.cpp -Iincludes -o build/main.o

##LIBRARY_ OBJECT_ FILES START
$(B1)/board.o : $(I1)/board.cpp
	# g++ -E $(I1)/board.cpp -Iincludes > board
	g++ -c $(I1)/board.cpp -Iincludes -o $(B1)/board.o

$(B1)/exceptions.o : $(I1)/exceptions.cpp
	g++ -c $(I1)/exceptions.cpp -Iincludes -o $(B1)/exceptions.o

$(B1)/ludo_box.o : $(I1)/ludo_box.cpp
	g++ -c $(I1)/ludo_box.cpp -Iincludes -o $(B1)/ludo_box.o

$(B1)/ludo_goti.o : $(I1)/ludo_goti.cpp
	# g++ -E $(I1)/ludo_goti.cpp -Iincludes > ludo_goti
	g++ -c $(I1)/ludo_goti.cpp -Iincludes -o $(B1)/ludo_goti.o

$(B1)/utilities.o : $(I1)/utilities.cpp
	g++ -c $(I1)/utilities.cpp -Iincludes -o $(B1)/utilities.o

$(B1)/ludo_coords.o : $(I1)/ludo_coords.cpp
	g++ -c $(I1)/ludo_coords.cpp -Iincludes -o $(B1)/ludo_coords.o	
##LIBRARY_ OBJECT_ FILES END

##Creating Libraries START
create_utilities_lib : $(UTIL_OBJECT_FILES)
	ar rvs libs/libutil.a $(UTIL_OBJECT_FILES)

create_static_lib : $(LIBS_OBJECT_FILES)
	ar rvs libs/libludo_static.a $(LIBS_OBJECT_FILES)

create_dynamic_lib : $(LIBS_OBJECT_FILES)
	echo "TODO"

##Creating Libraries END

clean:
	rm build/main.o build/object_files/*.o build/game*
