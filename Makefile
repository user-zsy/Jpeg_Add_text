
.PHONY: all obj clean

SRC+=$(wildcard *.c)

TMPOBJ:=$(SRC:%.c=%.o)

APP_FLAGS:=-L./lib/ -ljpeg 

COMPLIER_FLAGS:= -g -Wall -fPIC -I./include

OBJ:=Jpeg2bmp


CC=gcc
#CC=arm-linux-gnueabihf-gcc
obj: $(OBJ)
$(OBJ) : $(TMPOBJ)
	$(CC) -o $@  $^ $(APP_FLAGS)

$(TMPOBJ) : %.o:%.c
	$(CC) $(COMPLIER_FLAGS) -c $^ -o $@

clean:
	rm -f $(TMPOBJ)
	rm -f $(OBJ)
