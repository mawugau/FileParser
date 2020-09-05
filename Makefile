CC:=gcc
SRC:=parser.c
EXE:=parser.exe


all:
	$(CC) $(SRC) -o $(EXE)

clean:
	@rm -rf $(EXE)