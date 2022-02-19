CFLAGS=-std=gnu99
FILES=hinfosvc.c
PROJ=hinfosvc

all : $(PROJ)

$(PROJ) : $(FILES)
		gcc $(CFLAGS) -o $(PROJ) $(FILES)

clean :
	rm -f *.o $(PROJ)

run :
	./ifjComp 
