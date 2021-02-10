default: clean selected

clean:
	rm -f *jpg *png

selected:
	convert -crop 300x300+790+490 -gamma 1.25 \
		orig/*274.jpg board-comps.jpg
	convert -rotate 5 -crop 1000x350+420+590 -gamma 1.25 \
		orig/*276.jpg board-orange.jpg
	convert -crop 1150x400+360+550 -gamma 1.25 \
		orig/*287.jpg board-black.jpg
	convert -crop 1100x600+450+560 orig/*292.jpg board-conns.jpg

# This is the end of the file.
