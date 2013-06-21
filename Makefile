CC = g++

WARNINGS = -pedantic -Wall
CXXFLAGS = -lm -O3 $(WARNINGS)

ACONSTRUIR = main main_testing reconocedor

SRCPATH = src_cpp/
OBJPATH = obj/
EXEPATH = ejecutables/

all : tp3 test

$(OBJPATH)%.o : $(SRCPATH)%.cpp
	        $(CC) $(CXXFLAGS) -c $< -o $@

tp3 : $(OBJPATH)main.o $(OBJPATH)reconocedor.o
	        $(CC) $(OBJPATH)main.o $(OBJPATH)reconocedor.o -o $(EXEPATH)tp3.exe

test : $(OBJPATH)main_testing.o $(OBJPATH)reconocedor.o 
	        $(CC) $(OBJPATH)main_testing.o $(OBJPATH)reconocedor.o -o $(EXEPATH)test.exe

remake : clean all

clean :
	        rm -f $(OBJPATH)*.o $(EXEPATH)*.exe
