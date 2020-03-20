IDIR =include
CC=g++
CFLAGS=-I$(IDIR) -O2 -std=c++17

ODIR=src/obj
SDIR=src

LIBS=-lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

_DEPS = Core.hpp Cmd.hpp Commands.hpp Config.hpp Menu.hpp Game.hpp Tetris.hpp Tetrion.hpp Randomizer.hpp Events.hpp Hitreg.hpp Filesys.hpp Font.hpp Keys.hpp Log.hpp Media.hpp Music.hpp Render.hpp Sound.hpp Timer.hpp Vec2.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o Core.o Core_Console.o Cmd.o Commands.o Config.o Menu.o Game.o Tetris.o Tetrion.o Randomizer.o Events.o Hitreg.o Filesys.o Font.o Keys.o Log.o Media.o Music.o Render.o Sound.o Timer.o Vec2.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

OUTPUT = tspace

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<
		
debug: $(OBJ)
	$(CC) -g -o $(OUTPUT) $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
