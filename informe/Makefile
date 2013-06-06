CC = pdflatex

ACONSTRUIR = informe

NECESITADOS = informe enunciado introduccion_teorica conclusion resultados codigo desarrollo tablas testeo

SRCPATH = tex/

SRCFILES = $(patsubst %,$(SRCPATH)%.tex, $(NECESITADOS))


all : $(SRCFILES) clean
	cd tex; \
		$(CC) $(ACONSTRUIR).tex ; \
		$(CC) $(ACONSTRUIR).tex 
	mv $(SRCPATH)$(ACONSTRUIR).pdf $(ACONSTRUIR).pdf

clean :
	rm -f *.pdf $(SRCPATH)*.aux $(SRCPATH)*.log 
