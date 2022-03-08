CFLAGS=-std=gnu99
FILES=hinfosvc.c
PROJ=hinfosvc

all : $(PROJ)

$(PROJ) : $(FILES)
		gcc $(CFLAGS) -o $(PROJ) $(FILES)

clean :
	rm -f *.o $(PROJ) xondru18.zip
pack :
	zip xondru18 Makefile hinfosvc.c Readme.md

run :
	./$(PROJ) 1234
