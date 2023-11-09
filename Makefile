INCLUDE = include/
SRC     = src/
OBJ     = obj/
FLAGS   = -I $(INCLUDE)

CXX = g++

SOURCES = $(wildcard $(SRC)/*.cpp)
OBJECTS = $(foreach x,$(SOURCES), $(OBJ)/$(x).o)

all: list

run: list
	./list $(ARGS)

list: $(OBJECTS)
	$(CXX) -I $(INCLUDE) -o $@ $(OBJECTS)

$(OBJ)/%.o: %
	mkdir -p $(dir $@)
	$(CXX) -I $(INCLUDE) -o $@ -c $<

clean:
	rm -r $(OBJECTS) list *.png *.dot
