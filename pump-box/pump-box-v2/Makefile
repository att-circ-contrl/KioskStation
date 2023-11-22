# "Kiosk Station" NHP Game Kiosk - Pump box release Makefile.

default: helpscreen

helpscreen:
	@echo ""
	@echo "Targets:  release"
	@echo ""


SRCBOX=pump-box-v2
DSTBOX=pump-cable-adapter

SRCFOB=pump-fob-v2
DSTFOB=pump-fob

UPATH=../utils
DSTPATH=../release-v2

clean:
	rm -f $(DSTPATH)/*

release: clean release-adapter release-fob

release-adapter:
	cd boards/gerber-adapter; \
		../../$(UPATH)/pcb2seeed.sh $(SRCBOX) $(DSTBOX); \
		mv *zip ../../$(DSTPATH)/$(DSTBOX)-pcb.zip
	cd boards/gerber-adapter; \
		../../$(UPATH)/pcb2png.sh $(SRCBOX) \
		$(DSTBOX)-layout.png; mv *png ../../$(DSTPATH)/
	cp notes/$(SRCBOX)-bom.txt $(DSTPATH)/$(DSTBOX)-bom.txt
	cd meshes; zip ../$(DSTPATH)/$(DSTBOX)-meshes.zip case-mockup*stl

release-fob:
	cp notes/$(SRCFOB)-bom.txt $(DSTPATH)/$(DSTFOB)-bom.txt
	cd boards/gerber-fob; \
		../../$(UPATH)/pcb2seeed.sh $(SRCFOB) $(DSTFOB); \
		mv *zip ../../$(DSTPATH)/$(DSTFOB)-pcb.zip
	cd boards/gerber-fob; \
		../../$(UPATH)/pcb2png.sh $(SRCFOB) \
		$(DSTFOB)-layout.png; mv *png ../../$(DSTPATH)/
	cd meshes; zip ../$(DSTPATH)/$(DSTFOB)-meshes.zip $(SRCFOB)*stl
	cd iges; zip ../$(DSTPATH)/$(DSTFOB)-iges.zip $(SRCFOB)*iges

#
# This is the end of the file.
