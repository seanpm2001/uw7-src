include $(CMDRULES)
include ../../dlpi.mkf
BIN=tc_attach
OBJ=tc_attach.o

all build : ${BIN}

${BIN} : ${OBJ}  
	$(CC) -o $@ $? ${TET_OBJ} ${LIBS} 
	cp ${BIN} ${DLPI_BIN}/${BIN}

clean: 
	$(RM) $(OBJ) $(BIN)

