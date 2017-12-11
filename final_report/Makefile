# pdflatex needs to be run twice to ensure that the bibliography is updated properly.
all:
	pdflatex report.tex
	biber report
	pdflatex report.tex
clean:
	rm *.aux
	rm *.bbl
	rm *.bcf
	rm *.blg
	rm *.log
	rm *.pdf
	rm *.run.xml
