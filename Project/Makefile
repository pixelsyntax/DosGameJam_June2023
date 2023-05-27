PROJECT=DIVER
DJGPP_GCC=i586-pc-msdosdjgpp-gcc
DOSBOX=dosbox

default: linux_test


clean:
	rm -r Bin
	rm -r Assets

dirs:
	mkdir -p Bin/DOS
	mkdir -p Bin/Linux
	mkdir -p Assets

assets: dirs
	cp Gfx/*.bmp Assets/

linux: dirs
	gcc Src/*.c -o Bin/Linux/$(PROJECT) -lm -lSDL2

linux_test: linux assets
	Bin/Linux/$(PROJECT)

dos_resources: dirs
	cp Resources/CWSDPMI.EXE Bin/DOS/
	cp -r Assets Bin/DOS/ASSETS

dos: dirs dos_resources
	$(DJGPP_GCC) Src/*.c -o Bin/DOS/$(PROJECT).EXE -lm 

dos_test: dos
	$(DOSBOX) Bin/DOS/DIVER.EXE
