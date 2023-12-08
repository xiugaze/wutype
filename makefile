# /**
#  * @file makefile
#  * @author Caleb Andreano (andreanoc@msoe.edu)
#  * @class CPE2600-121
#  * 
#  * Assignment: Lab Wk 15
#  * @date 2023-12-07
#  */

CC=gcc                      # c compiler
CFLAGS=-c -Wall -ggdb        # compiler flags
LDFLAGS= -l ncursesw                   # linker arguments
SOURCES=main.c wutype.c utils.c
OBJECTS=$(patsubst %.c,build/%.o,$(SOURCES))
DEPS=$(patsubst %.o,%.d,$(OBJECTS))
EXECUTABLE=build/wutype

all: $(EXECUTABLE)

# pull in dependency info for *existing* .o files
-include $(DEPS)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	# ./$@

build/%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
	$(CC) -MM $< > build/$*.d

clean:
	rm -rf build/*.o build/*.d $(EXECUTABLE)




