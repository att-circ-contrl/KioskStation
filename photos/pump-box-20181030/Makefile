default: clean selected

clean:
	rm -f *jpg *png

selected:
	convert -crop 1400x300+300+730 -gamma 1.5 orig/*239.jpg plug-started.jpg
	convert -crop 1200x400+500+480 -gamma 1.5 orig/*242.jpg plug-midway.jpg
	convert -crop 1000x300+500+580 -gamma 1.5 orig/*245.jpg plug-done.jpg
	convert -crop 900x500+550+680 -gamma 1.25 orig/*253.jpg board-done.jpg


# This is the end of the file.
