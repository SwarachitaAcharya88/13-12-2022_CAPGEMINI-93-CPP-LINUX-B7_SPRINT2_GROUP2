SRCDIR = src
OBJDIR = obj
INCLUDEDIR = include
BINDIR = tools/valgrind

INCLUDEFLAGS = -I${INCLUDEDIR}
CPPFLAGS = -ansi -c -g -Wall -DDEBUG -std=gnu++11 ${INCLUDEFLAGS}

final: sw_common.o sw_client.o sw_client_main.o sw_server.o sw_server_main.o
	g++ -o ${BINDIR}/final ${OBJDIR}/sw_common.o ${OBJDIR}/sw_client.o ${OBJDIR}/sw_client_main.o ${OBJDIR}/sw_server.o ${OBJDIR}/sw_server_main.o

sw_common.o:${SRCDIR}/sw_common.cpp
	g++ -o ${OBJDIR}/sw_common.o ${CPPFLAGS} ${SRCDIR}/sw_common.cpp

sw_client.o:${SRCDIR}/sw_client.cpp
	g++ -o ${OBJDIR}/sw_client.o ${CPPFLAGS} ${SRCDIR}/sw_client.cpp

sw_client_main.o:${SRCDIR}/sw_client_main.cpp
	g++ -o ${OBJDIR}/sw_client_main.o ${CPPFLAGS} ${SRCDIR}/sw_client_main.cpp

sw_server.o:${SRCDIR}/sw_server.cpp
	g++ -o ${OBJDIR}/sw_server.o ${CPPFLAGS} ${SRCDIR}/sw_server.cpp

sw_server_main.o:${SRCDIR}/sw_server_main.cpp
	g++ -o ${OBJDIR}/sw_server_main.o ${CPPFLAGS} ${SRCDIR}/sw_server_main.cpp



valgrind :final
	valgrind --leak-check=full --show-reachable=yes ${BINDIR}/main 2> ${VALGRIND}/valgrind.txt

clean :
	echo "cleaning up .o and exes"
	rm ${BINDIR}/final ${OBJDIR}/sw_common.o ${OBJDIR}/sw_client.o ${OBJDIR}/sw_client_main.o ${OBJDIR}sw_server.o ${OBJDIR}sw_server_main.o
