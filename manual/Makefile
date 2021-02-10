# Kiosk manual - Makefile
# Written by Christopher thomas.


BASENAME=kiosk-manual

DOCBITS=\
	$(BASENAME)-routine.tex		\
	$(BASENAME)-setup.tex		\
	$(BASENAME)-pumpbox.tex		\
	$(BASENAME).tex


default: force

force: clean $(BASENAME).pdf

clean: tclean
	rm -f $(BASENAME).pdf

tclean:
	rm -f $(BASENAME).log $(BASENAME).aux $(BASENAME).toc
	rm -f texput.*


$(BASENAME).pdf: $(DOCBITS)
	pdflatex $(BASENAME).tex
	pdflatex $(BASENAME).tex
	make tclean


# This is the end of the file.
