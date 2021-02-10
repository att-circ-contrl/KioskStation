default: clean selected

clean:
	rm -f *jpg *png

selected:
	convert -crop 600x900+100+50 orig/*15.jpg face-before.jpg
	convert -crop 600x900+0+124 orig/*1c.jpg hatch.jpg
	convert -crop 600x800+0+220 orig/*57.jpg lightfilts.jpg
	convert -crop 700x950+50+70 orig/*5d.jpg cameras.jpg
	convert -crop 650x650+90+270 orig/*1f.jpg bay-before.jpg
	convert -crop 650x650+120+320 orig/*22.jpg bay-stripped.jpg
	convert -crop 1000x400+20+180 orig/*27.jpg bay-parts.jpg
	convert -crop 800x700+80+30 orig/*2c.jpg ts-installed.jpg
	convert -rotate 130 -crop 350x500+350+320 -gamma 1.5 orig/*2d.jpg ts-labels.jpg
	convert -crop 1024x600+0+120 orig/*_3.jpg fan-parts.jpg
	convert -crop 700x600+40+150 orig/*2f.jpg fan-screws.jpg
	convert -crop 500x800+150+50 orig/*_e.jpg fan-assembled.jpg
	convert -crop 700x800+40+120 orig/*34.jpg fan-installed.jpg
	convert -crop 600x950+80+40 -rotate 270 orig/*28.jpg air-taped.jpg
	convert -crop 600x800+100+150 orig/*29.jpg air-batting.jpg
	convert -crop 700x700+30+250 orig/*36.jpg air-installed.jpg
	convert -crop 700x550+130+120 -gamma 1.5 orig/*3a.jpg power.jpg
	convert -crop 1000x650+12+0 orig/*3d.jpg small-mon.jpg
	convert orig/*43.jpg small-mon-gateway.jpg
	convert -crop 900x600+0+0 orig/*45.jpg unity-wiring.jpg
	convert -crop 1024x330+0+0 orig/*48.jpg ard-pbox-wiring.jpg
	convert -crop 700x320+0+0 orig/*4e.jpg ard-unity-placement.jpg
	convert -rotate 1 -crop 1000x300+10+20 -gamma 1.25 orig/*51.jpg led-box.jpg
	convert -rotate -0.5 -crop 900x300+70+0 -gamma 1.25 orig/*68.jpg led-ncam-placement.jpg
	convert -rotate -3 -crop 700x900+20+30 orig/*69.jpg ext-cabling.jpg
	convert -crop 750x900+0+0 orig/*6d.jpg pump.jpg
	convert -crop 700x800+0+70 orig/*6f.jpg cover-front.jpg
	convert -crop 768x900+0+120 -gamma 1.5 orig/*72.jpg cover-back.jpg


# This is the end of the file.
