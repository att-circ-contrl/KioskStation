# "Kiosk Station" NHP Game Kiosk - Pump box release Makefile.

default: helpscreen

helpscreen:
	@echo ""
	@echo "Targets:  release"
	@echo ""


SRCBASE=pump-box-v2
DSTBASE=pump-cable-adapter

UPATH=../utils
DSTPATH=../release-v2

clean:
	rm -f $(DSTPATH)/*

release: clean
	cd boards/gerber; ../../$(UPATH)/pcb2seeed.sh $(SRCBASE) $(DSTBASE); mv *zip ../../$(DSTPATH)/$(DSTBASE)-pcb.zip
	cd boards/gerber; ../../$(UPATH)/pcb2png.sh $(SRCBASE) $(DSTBASE)-layout.png; mv *png ../../$(DSTPATH)/
	cp notes/*bom.txt $(DSTPATH)/$(DSTBASE)-bom.txt
	cd meshes; zip ../$(DSTPATH)/$(DSTBASE)-meshes.zip case-mockup*stl

# FIXME - Add IGES models when we have them.

#
# This is the end of the file.
