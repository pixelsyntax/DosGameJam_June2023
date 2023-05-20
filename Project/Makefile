PROJECT=DIVER
DJGPP_GCC=i586-pc-msdosdjgpp-gcc
DOSBOX=dosbox

clean:
	rm -r Bin

dirs:
	mkdir -p Bin/DOS

dos_resources: dirs
	cp Resources/CWSDPMI.EXE Bin/DOS/

dos: dirs dos_resources
	$(DJGPP_GCC) Src/*.c -o Bin/DOS/$(PROJECT).EXE

dos_test: dos
	$(DOSBOX) Bin/DOS/DIVER.EXE
