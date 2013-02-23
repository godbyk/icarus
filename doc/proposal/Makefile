FILE=proposal

all:	view

dvi:
	latex $(FILE)
	latex $(FILE)
	latex $(FILE)

pdf:	
	#dvipdf $(FILE).dvi
	pdflatex $(FILE).latex
	#bibtex $(FILE)
	pdflatex $(FILE).latex
	pdflatex $(FILE).latex

view:	pdf
	evince $(FILE).pdf
	
web:	pdf
	cp $(FILE).pdf $(HOME)/WWW/
	chmod 644 $(HOME)/WWW/$(FILE).pdf

clean:
	rm -f $(FILE).aux $(FILE).dvi $(FILE).log $(FILE).pdf $(FILE).bbl $(FILE).blg $(FILE).toc $(HOME)/WWW/$(FILE).pdf
