#include ../../setenv.mk
SHELL := cmd
.PHONY: chm pdf html

all: pdf

doxy:
	taskkill /FI "windowtitle eq nmopengl"
	doxygen
	start ""  %CD%\html\nmopengl.chm 



chm:
	doxygen
	utf2win.bat
	taskkill /FI "windowtitle eq nmopengl"
	-cd html && hhc index.hhp
	copy html\*.chm .

html:
	doxygen

pdf:
	doxygen
	cd latex &&	call make.bat
	copy .\latex\refman.pdf .\nmopengl.pdf

clean:
	-rm -f -r -v html xml latex nmopengl.chm nmopengl.pdf *.html .*

open:
	www.bat

	#http://rc-module.github.io/nmopengl
	#pdf
	#cp ./latex/refman.pdf ./html/nmopengl.pdf
	#copy nmopengl.pdf  html\nmopengl.pdf
	#nmopengl.html ipp-nmopengl.html
	#copy ipp-nmopengl.html html\ipp-nmopengl.html
	#copy nmopengl.html html\nmopengl.html

github: clean pdf chm
	doxygen
	copy nmopengl.chm html\nmopengl.chm /Y
	copy nmopengl.pdf html\nmopengl.pdf	/Y
	7za a html\nmopengl.zip nmopengl.chm &&\
	cd html &&\
	git init . && git checkout --orphan gh-pages &&\
	git add * &&\
	git commit -m "documentation-for-gh-pages" &&\
	git remote add origin https://github.com/RC-MODULE/nmOpenGL.git  &&\
	git push -u -f origin gh-pages


github0: clean pdf chm
	copy nmopengl.chm html\nmopengl.chm /Y
	copy nmopengl.pdf html\nmopengl.pdf	/Y
	7za a html\nmopengl.zip nmopengl.chm &&\
	cd html &&\
	git init . && git checkout --orphan gh-pages &&\
	git add * &&\
	git commit -m "documentation-for-gh-pages" &&\
	git remote add origin https://github.com/RC-MODULE/nmOpenGL.git  &&\
	git push -u -f origin gh-pages

github-html:
	cd html &&\
	git init . && git checkout --orphan gh-pages &&\
	git add * &&\
	git commit -m "documentation-for-gh-pages" &&\
	git remote add origin https://github.com/RC-MODULE/nmOpenGL.git  &&\
	git push -u -f origin gh-pages


ipp:
	doxygen DoxyIPP

ipp-nmopengl.html:
	lua ipp-nmopengl.lua
	xsltproc -o ipp-nmopengl.html ipp-nmopengl.xsl ipp-nmopengl.xml

nmopengl.html:
	lua summary.lua
	xsltproc -o nmopengl.html nmopengl.xsl nmopengl.xml

git-summary: nmopengl.html ipp-nmopengl.html
	copy ipp-nmopengl.html html\ipp-nmopengl.html
	copy nmopengl.html html\nmopengl.html
	cd html &&\
	git add * &&\
	git commit -m "documentation-for-gh-pages" &&\
	git push -u -f origin gh-pages
