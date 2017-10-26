# @Author: Moustapha KEBE <kebson>
# @Date:   2017-10-12T11:33:21+02:00
# @Email:  mktapha@gmail.com
# @Last modified by:   kebson
# @Last modified time: 2017-10-26T08:02:13+02:00



CC := gcc
CFLAGS := -g -Wall -std=gnu99 -fgnu89-inline
LINKER :=
ENABLEDLL := -DBUILD_CHESSLIB_DLL
NDEBUG := -DNDEBUG
DLL := chesslib.dll
ELF := jeuconsole
INC_W_LEVEL := -Wextra -pedantic
BLDFOLDER := build
TESTFOLDER := tests
CHESSLIB := chesslib.c
AIC := deplacement.c
prefix := /usr/local

# Build Linux cli ELF #
jeuconsole: jeuconsole.c $(AIC) $(CHESSLIB)
	if [ ! -e $(BLDFOLDER) ]; then mkdir $(BLDFOLDER); fi \
	&& $(CC) $(CFLAGS) $(NDEBUG) $< $(CHESSLIB) $(AIC) -o $@ $(LINKER); mv $@ $(BLDFOLDER)


# Same thing just with more warnings #
jeuconsoleWall: jeuconsole.c $(CHESSLIB) $(AIC)
	if [ ! -e $(BLDFOLDER) ]; then mkdir $(BLDFOLDER); fi \
	&& $(CC) $(CFLAGS) $(NDEBUG) $(INC_W_LEVEL) $< $(CHESSLIB) $(AIC) -o $@ $(LINKER); mv $@ $(BLDFOLDER)

# Build static library archive of ChessLib for Linux to be linked to your program at compile time #
chesslib: $(CHESSLIB)
	$(CC) -c $(CFLAGS) $(NDEBUG) $<; \
	ar csq libchesslib.a chesslib.o; \
	rm chesslib.o

# Build Linux ELF with debugging flags enabled #
debug: jeuconsole.c $(CHESSLIB) $(AIC)
	if [ ! -e $(BLDFOLDER) ]; then mkdir $(BLDFOLDER); fi \
	&& $(CC) $(CFLAGS) $< $(CHESSLIB) $(AIC) -o $@ $(LINKER); mv $@ $(BLDFOLDER)

# Build Windows ELF
exe: jeuconsole.c $(CHESSLIB) $(AIC)
	$(CC) $(NDEBUG) -o jeuconsole.exe $^

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
	rm libchesslib.a

cleantxt:
	rm -rf build/*.txt \
	rm *.txt

install: jeuconsole
	install -m 0755 $(BLDFOLDER)/$< $(prefix)/bin
