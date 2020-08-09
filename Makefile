CPP = g++
AR = ar
CFLAGS = -D__MBED__ -I./include -I./MD_MAX72XX/src -I./MD_Parola/src

bin: bin/helloworld bin/display bin/ambulance bin/customchars bin/zone_display bin/watch


SRC_MAX72XX = $(wildcard ./MD_MAX72XX/src/*.cpp)
SRC_PAROLA = $(wildcard ./MD_Parola/src/*.cpp)
INC_MAX72XX = $(wildcard ./MD_MAX72XX/src/*.h)
INC_PAROLA = $(wildcard ./MD_Parola/src/*.h)
OBJ_MAX72XX = $(subst ./MD_MAX72XX/src, ./obj, $(SRC_MAX72XX:.cpp=.o))
OBJ_PAROLA = $(subst ./MD_Parola/src, ./obj, $(SRC_PAROLA:.cpp=.o))


obj/main.o : main.cpp 
	$(CPP) -c -o $@ $< $(CFLAGS)

lib/libParola.a : $(OBJ_PAROLA)
	$(AR) rcs $@ $^

lib/libMax72xx.a : $(OBJ_MAX72XX)
	$(AR) rcs $@ $^

obj/%.o: ./MD_MAX72XX/src/%.cpp
	$(CPP) $(CFLAGS) -c $< -o $@

obj/%.o: ./MD_Parola/src/%.cpp
	$(CPP) $(CFLAGS) -c $< -o $@



obj/Parola_HelloWorld.o: ./examples/Parola_HelloWorld/Parola_HelloWorld.ino
	$(CPP) -c -o $@ -x c++ $< $(CFLAGS)
bin/helloworld: obj/Parola_HelloWorld.o obj/main.o lib/libParola.a lib/libMax72xx.a
	$(CPP) $^ -o $@ $(CFLAGS) -L./lib -lParola -lMax72xx

obj/Parola_Display.o: ./examples/Parola_Display/Parola_Display.ino
	$(CPP) -c -o $@ -x c++ $< $(CFLAGS)
bin/display: obj/Parola_Display.o obj/main.o lib/libParola.a lib/libMax72xx.a
	$(CPP) $^ -o $@ $(CFLAGS) -L./lib -lParola -lMax72xx

obj/Parola_Ambulance.o: ./examples/Parola_Ambulance/Parola_Ambulance.ino
	$(CPP) -c -o $@ -x c++ $< $(CFLAGS)
bin/ambulance: obj/Parola_Ambulance.o obj/main.o lib/libParola.a lib/libMax72xx.a
	$(CPP) $^ -o $@ $(CFLAGS) -L./lib -lParola -lMax72xx

obj/Parola_CustomChars.o: ./examples/Parola_CustomChars/Parola_CustomChars.ino
	$(CPP) -c -o $@ -x c++ $< $(CFLAGS)
bin/customchars: obj/Parola_CustomChars.o obj/main.o lib/libParola.a lib/libMax72xx.a
	$(CPP) $^ -o $@ $(CFLAGS) -L./lib -lParola -lMax72xx

obj/Parola_Zone_Display.o: ./examples/Parola_Zone_Display/Parola_Zone_Display.ino
	$(CPP) -c -o $@ -x c++ $< $(CFLAGS)
bin/zone_display: obj/Parola_Zone_Display.o obj/main.o lib/libParola.a lib/libMax72xx.a
	$(CPP) $^ -o $@ $(CFLAGS) -L./lib -lParola -lMax72xx

obj/watch.o: ./examples/watch/watch.ino
	$(CPP) -c -o $@ -x c++ $< $(CFLAGS)
bin/watch: obj/watch.o obj/main.o lib/libParola.a lib/libMax72xx.a
	$(CPP) $^ -o $@ $(CFLAGS) -L./lib -lParola -lMax72xx

clean :
	rm -rf obj/* lib/*

