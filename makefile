CC=gcc                      # c compiler
CFLAGS=-c -Wall -ggdb        # compiler flags
LDFLAGS= -l ncursesw                   # linker arguments
SOURCES=main.c wutype.c utils.c
OBJECTS=$(patsubst %.c,build/%.o,$(SOURCES))
DEPS=$(patsubst %.o,%.d,$(OBJECTS))
EXECUTABLE=build/main

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




