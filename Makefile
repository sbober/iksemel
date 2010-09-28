SRC=dom/dom.c sax/sax.c iks/iks.c ikstack/ikstack.c utility/utility.c
OBJ=$(SRC:.c=.o)

OUT=libiksemel.a

INCLUDES=-I.

CFLAGS=-Os

CC=gcc
default: $(OUT)


.c.o:
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	ar rcs $(OUT) $(OBJ)


clean:
	rm -f $(OBJ) $(OUT)



