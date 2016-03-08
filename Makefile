#Kyler Kiminki
#kkiminki
#cmps104a
#Makefile
#Makefile for asg2

GCC        = g++ -g -O0 -Wall -Wextra -std=gnu++11
MKDEP      = g++ -MM -std=gnu++11
VALGRIND   = valgrind --leak-check=full --show-reachable=yes
NOINCLUDE  = ci clean spotless
NEEDINCL   = ${filter ${NOINCLUDE}, ${MAKECMDGOALS}}
CPP        = g++ -g -O0 -Wall -Wextra -std=gnu++11
FLEX       = flex --header-file=${LEXHDR} --outfile=${LEXCPP}
BISON      = bison --defines=${PARSEHDR} --output=${PARSECPP}

MKFILE     = Makefile
DEPFILE    = Makefile.dep
MODULES    = lyutils auxlib stringset astree symtable
HDRSRC     = ${MODULES:=.h} symbol.h
CPPSRC     = ${MODULES:=.cpp} main.cpp
FLEXSRC    = lexer.l
BISONSRC   = parser.y
LEXHDR     = yylex.h
PARSEHDR   = yyparse.h
LEXCPP     = yylex.cpp
PARSECPP   = yyparse.cpp
CGENS      = ${LEXCPP} ${PARSECPP}
ALLGENS    = ${LEXHDR} ${PARSEHDR} ${CGENS}
EXECBIN    = oc
ALLCSRC     = ${CPPSRC} ${CGENS}
OBJECTS    = ${ALLCSRC:.cpp=.o}
LEXOUT     = yylex.output
PARSEOUT   = yyparse.output
REPORTS    = ${LEXOUT} ${PARSEOUT}
MODSRC     = ${foreach MOD, ${MODULES}, ${MOD}.h ${MOD}.cpp}
MISCSRC    = ${filter-out ${MODSRC}, ${HDRSRC} ${CPPSRC}}
ALLSRC     = README ${FLEXSRC} ${BISONSRC} ${MODSRC} ${MISCSRC} Makefile
TESTINS    = ${wildcard test*.in}
EXECTEST   = ${EXECBIN} -ly
LISTSRC    = ${ALLSRC} ${DEPSFILE} ${PARSEHDR}

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${CPP} -o${EXECBIN} ${OBJECTS}

%.o : %.cpp bison
	${CPP} -c $<

${LEXCPP} : ${FLEXSRC}
	${FLEX} ${FLEXSRC} 2>${LEXOUT}
	- grep -v '^  ' ${LEXOUT}

bison : ${PARSECPP} ${PARSEHDR}

${PARSECPP} ${PARSEHDR} : ${BISONSRC}
	${BISON} ${BISONSRC}
	echo -e '1,$$!cpp -fpreprocessed -dD\nw\nq\n' | ex ${PARSEHDR}

ci :  ${EXECBIN}
	- checksource ${ALLSRC}
	git add .
	git commit

cioverride:
	git add .
	git commit

clean :
	-rm ${OBJECTS} ${ALLGENS} ${REPORTS} ${DEPSFILE}

spotless : clean
	- rm ${EXECBIN} List.*.ps List.*.pdf

deps : ${ALLSRC}
	@ echo "# ${DEPSFILE} created `date` by ${MAKE}" >${DEPSFILE}
	${MKDEPS} ${ALLSRC} >> ${DEPSFILE}

${DEPFILE} :
	@ touch ${DEPSFILE}
	${MAKE} --no-print-directory deps

tests : ${EXECBIN}
	touch ${TESTINS}
	make --no-print-directory ${TESTINS:.in=.out}

%.out %.err : %.in ${EXECBIN}
	${VALGRIND} --log-file=$*.log ${EXECTEST} $< 1>$*.out 2>$*.err; \
	echo EXIT STATUS = $$? >>$*.log

again :
	gmake --no-print-directory spotless deps ci all lis

testsub: ${EXECBIN}
	cp auxlib.* ./testsub
	cp lexer.l ./testsub
	cp main.cpp ./testsub
	cp stringset.* ./testsub
	cp parser.y ./testsub
	cp Makefile ./testsub
	cp lyutils.* ./testsub
	cp astree.* ./testsub
	cp symtable.* ./testsub
	cp symbol.h ./testsub

space: ${OBJECTS}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	${EXECBIN}

submit : 
	submit cmps104a-wm.f15 asg5 ${CPPSRC} ${HDRSRC} lexer.l parser.y Makefile README
ifeq "${NEEDINCL}" ""
include ${DEPSFILE}
endif

