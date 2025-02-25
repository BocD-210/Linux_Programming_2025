
CUR_DIR := .
BIN_DIR := $(CUR_DIR)/bin
INC_DIR := $(CUR_DIR)/inc
OBJ_DIR := $(CUR_DIR)/obj
SRC_DIR := $(CUR_DIR)/src
LIB_DIR := $(CUR_DIR)/lib
LIB_STATIC_DIR := $(LIB_DIR)/static
LIB_SHARED_DIR := $(LIB_DIR)/shared

C_FLAGS = -I $(INC_DIR)

Create_objs:
	gcc -c -fPIC $(SRC_DIR)/helloDo.c -o $(OBJ_DIR)/helloDo.o -I $(INC_DIR)
	gcc -c -fPIC $(SRC_DIR)/helloWorld.c -o $(OBJ_DIR)/helloWorld.o -I $(INC_DIR)
	gcc -c $(SRC_DIR)/main.c -o $(OBJ_DIR)/main.o -I $(INC_DIR)

Create_SharedLibs:
	gcc -shared $(OBJ_DIR)/helloDo.o $(OBJ_DIR)/helloWorld.o -o $(LIB_SHARED_DIR)/libhello.so

Create_StaticLibs:
	ar rcs $(LIB_STATIC_DIR)/libhello.a $(OBJ_DIR)/helloDo.o $(OBJ_DIR)/helloWorld.o

all_Shared_Libs: Create_objs Create_SharedLibs
	gcc  $(OBJ_DIR)/main.o -L$(LIB_SHARED_DIR) -lhello -o $(BIN_DIR)/linked_Shared

all_Static_Libs: Create_objs Create_StaticLibs
	gcc  $(OBJ_DIR)/main.o  -L$(LIB_STATIC_DIR) -lhello -o $(BIN_DIR)/statically-linked

clean:
	rm -rf $(OBJ_DIR)/*.o
	rm -rf $(LIB_SHARED_DIR)/*.so
	rm -rf $(LIB_STATIC_DIR)/*.a
	rm -rf $(BIN_DIR)/linked_Shared
	rm -rf $(BIN_DIR)/statically-linked


