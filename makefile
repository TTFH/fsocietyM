all: main

.PHONY: all clean_bin clean

HDIR    = headers
CPPDIR  = cpp
ODIR    = obj

MODULOS = aes base64 random encrypter factory priority_queue embedded_file

HS   = $(MODULOS:%=$(HDIR)/%.h)
CPPS = $(MODULOS:%=$(CPPDIR)/%.cpp)
OS   = $(MODULOS:%=$(ODIR)/%.o)

OBJETIVO = main
EJECUTABLE = fsocietyM

CC = g++
CCFLAGS = -Wall -Werror -I$(HDIR) -Wno-unused-result -fconcepts

$(ODIR)/$(OBJETIVO).o:$(OBJETIVO).cpp
	$(CC) $(CCFLAGS) -c $< -o $@

$(ODIR)/%.o: $(CPPDIR)/%.cpp $(HDIR)/%.h
	$(CC) $(CCFLAGS) -c $< -o $@

$(OBJETIVO): $(ODIR)/$(OBJETIVO).o $(OS) 
	$(CC) $(CCFLAGS) $^ -o $(EJECUTABLE)

clean_bin:
	rm -f $(EJECUTABLE) $(ODIR)/$(OBJETIVO).o $(OS)

clean: clean_bin
	@rm -f *~ $(HDIR)/*~ $(CPPDIR)/*~

check-syntax:
	gcc -o nul -S ${CHK_SOURCES}
