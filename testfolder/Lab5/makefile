all: main

# Objetivos que no son archivos
.PHONY: all clean_bin clean

# Directorios 
HDIR    = include
CPPDIR  = cpp
ODIR    = obj
TESTDIR = test

MODULOS = datatypes fabrica iproducto iempleado icliente iventa producto empleado cliente venta

# Lista de archivos, con directorio y extension
HS   = $(MODULOS:%=$(HDIR)/%.h)
CPPS = $(MODULOS:%=$(CPPDIR)/%.cpp)
OS   = $(MODULOS:%=$(ODIR)/%.o)

OBJETIVO = main
EJECUTABLE = main

# Compilador
CC = g++
# Opciones de compilacion
CCFLAGS = -Wall -Werror -I$(HDIR) -g

$(ODIR)/$(OBJETIVO).o:$(OBJETIVO).cpp
	$(CC) $(CCFLAGS) -c $< -o $@

# $@ se expande para tranformarse en el objetivo
# $< se expande para tranformarse en la primera dependencia
$(ODIR)/%.o: $(CPPDIR)/%.cpp $(HDIR)/%.h
	$(CC) $(CCFLAGS) -c $< -o $@

# $^ se expande para tranformarse en todas las dependencias 
$(EJECUTABLE): $(ODIR)/$(OBJETIVO).o $(OS) 
	$(CC) $(CCFLAGS) $^ -o $@

# Borrar binarios
clean_bin:
	rm -f $(EJECUTABLE) $(ODIR)/$(OBJETIVO).o $(OS)

# Borra binarios, resultados de ejecución y comparación, y copias de respaldo
clean: clean_bin
	@rm -f *~ $(HDIR)/*~ $(CPPDIR)/*~

check-syntax:
	gcc -o nul -S ${CHK_SOURCES}
