#
# TransFig makefile
#

all: false-progress-a.tex false-progress-b.tex 

# translation into epic

false-progress-a.tex: false-progress-a.fig Makefile
	fig2dev -L epic false-progress-a.fig > false-progress-a.tex
clean::
	rm -f false-progress-a.tex


# translation into epic

false-progress-b.tex: false-progress-b.fig Makefile
	fig2dev -L epic false-progress-b.fig > false-progress-b.tex
clean::
	rm -f false-progress-b.tex

