PROJECT=DIVER
DJGPP_GCC=i586-pc-msdosdjgpp-gcc
DOSBOX=dosbox

default: linux_test


clean:
	rm -r Bin

dirs:
	mkdir -p Bin/DOS
	mkdir -p Bin/Linux

linux: dirs
	gcc Src/*.c -o Bin/Linux/$(PROJECT) -lm -lSDL2

linux_test: linux
	Bin/Linux/$(PROJECT)

dos_resources: dirs
	cp Resources/CWSDPMI.EXE Bin/DOS/

dos: dirs dos_resources
	$(DJGPP_GCC) Src/*.c -o Bin/DOS/$(PROJECT).EXE -lm 

dos_test: dos
	$(DOSBOX) Bin/DOS/DIVER.EXE
