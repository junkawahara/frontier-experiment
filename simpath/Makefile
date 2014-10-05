all: simpath simpath-silent

simpath: simpath.c myheader.h
	gcc -o simpath -O3 simpath.c

simpath-silent: simpath.c myheader.h
	gcc -o simpath-silent -O3 simpath.c -DNO_OUTPUT

simpath.c: simpathtemp.c
	sed -e "s/#include \"gb_graph.h\"/#include \"myheader.h\"/g" \
	    -e "s/#include \"gb_save.h\"//g" \
        -e "s/fprintf/fprintxf/g" \
        simpathtemp.c >simpathtemp2.c
	sed -e "s/printf/myprintf/g" \
	    -e "s/fprintxf/fprintf/g" \
	    simpathtemp2.c >simpath.c
	rm simpathtemp2.c

simpathtemp.c: simpath.w
	-ctangle simpath.w - simpathtemp.c

simpath.w:
	wget http://www-cs-faculty.stanford.edu/~uno/programs/simpath.w
