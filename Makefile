CC = g++

WARNINGS = -pedantic -Wall
CXXFLAGS = -lm -O3 $(WARNINGS)

EJECUTABLE = tp3.exe
ACONSTRUIR = main reconocedor

SRCPATH = src_cpp/
OBJPATH = obj/
EXEPATH = ejecutables/

OBJFILES = $(patsubst %,$(OBJPATH)%.o, $(ACONSTRUIR))

all : tp2

$(OBJPATH)%.o : $(SRCPATH)%.cpp
	        $(CC) $(CXXFLAGS) -c $< -o $@

tp2 : $(OBJFILES) 
	        $(CC) $(OBJFILES) -o $(EXEPATH)$(EJECUTABLE)
		        
remake : clean all

clean :
	        rm -f $(OBJPATH)*.o $(EXEPATH)*.exe
