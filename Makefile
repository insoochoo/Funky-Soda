CXX = u++					# compiler
CXXFLAGS = -g -multi -Wall -Wno-unused-label -MMD -O2
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS2 = main.o config.o bank.o parent.o watCard.o watCardOffice.o groupoff.o vendingMachine.o nameServer.o truck.o bottlingPlant.o student.o printer.o
EXEC2 = soda

OBJECTS = ${OBJECTS2}				# all object files
DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"
EXECS = ${EXEC2}				# all executables

#############################################################

.PHONY : all clean

all : ${EXECS}					# build all executables

${EXEC2} : ${OBJECTS2}
	${CXX} ${CXXFLAGS} $^ -o $@


#############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f *.d *.o ${EXEC01} ${EXECS}
