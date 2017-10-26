# @Author: Moustapha KEBE <kebson>
# @Date:   2017-10-12T11:33:21+02:00
# @Email:  mktapha@gmail.com
# @Last modified by:   kebson
# @Last modified time: 2017-10-26T08:52:34+02:00



CC := gcc
CFLAGS := -g -Wall -std=gnu99 -fgnu89-inline
ENABLEDLL := -DBUILD_CHESSLIB_DLL
NDEBUG := -DNDEBUG
ELF := server
INC_W_LEVEL := -Wextra -pedantic
BLDFOLDER := build
TESTFOLDER := tests
CLIENT := client.c
prefix := /usr/local

# Build Linux cli ELF #
server: server.c $(CLIENT)
	if [ ! -e $(BLDFOLDER) ]; then mkdir $(BLDFOLDER); fi \
	&& $(CC) $(CFLAGS) $(NDEBUG) $< $(CLIENT) -o $@ ; mv $@ $(BLDFOLDER)


# Same thing just with more warnings #
serverWall: server.c $(CLIENT)
	if [ ! -e $(BLDFOLDER) ]; then mkdir $(BLDFOLDER); fi \
	&& $(CC) $(CFLAGS) $(NDEBUG) $(INC_W_LEVEL) $< $(CLIENT) -o $@ ; mv $@ $(BLDFOLDER)

# Build static library archive of ChessLib for Linux to be linked to your program at compile time #
client: $(CLIENT)
	$(CC) -c $(CFLAGS) $(NDEBUG) $<; \
	ar csq client.a client.o; \
	rm client.o

# Build Linux ELF with debugging flags enabled #
debug: server.c $(CLIENT)
	if [ ! -e $(BLDFOLDER) ]; then mkdir $(BLDFOLDER); fi \
	&& $(CC) $(CFLAGS) $< $(CLIENT) -o $@ ; mv $@ $(BLDFOLDER)

# Build Windows ELF
exe: server.c $(CLIENT)
	$(CC) $(NDEBUG) -o server.exe $^

# Run the Linux executables #
run:
	exec ./$(BLDFOLDER)/$(ELF)

runW:
	exec ./$(BLDFOLDER)/$(ELF)Wall

.PHONY: install clean  cleantxt
clean:
	rm -rf build tests \
	rm *.o \
	rm *.out \
	rm client.a

cleantxt:
	rm -rf build/*.txt \
	rm *.txt

install: server
	install -m 0755 $(BLDFOLDER)/$< $(prefix)/bin
