#CLIENTDIR := source/client
CLIENTDIR := testes/Teste_impressao_mapa
SERVERDIR := source/server
COMMONDIR := source/common
MENUDIR := testes/Teste_menu
#MENUDIR := testes/Teste_impressao_mapa

CC := gcc -std=c99
RM := rm -f
MK := mkdir -p
#Uncomment above for intense debugging
#CFLAGS := -Wall -Werror -Wconversion -Wextra
LDLIB := -lm -lallegro -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec

OUTPUTDIR := bin
LIBDIR := lib

CLIENTNAME := client
SERVERNAME := server
MENUNAME := menu

EXT := c
INC := -I lib -I $(COMMONDIR) 
#-I $(MUSICDIR) -I $(SAMPLEDIR) -I $(FONTSDIR) -I $(IMAGESDIR) -I $(BACKGROUNDDIR) -I $(CHARACTERDIR)

CLIENTSOURCES := $(shell find $(CLIENTDIR) -type f -name *.$(EXT))
SERVERSOURCES := $(shell find $(SERVERDIR) -type f -name *.$(EXT))
LIBSOURCES := $(shell find $(LIBDIR) -type f -name *.$(EXT))
COMMONSOURCES := $(shell find $(COMMONDIR) -type f -name *.$(EXT))
MENUSOURCES := $(shell find $(MENUDIR) -type f -name *.$(EXT))


CLIENTOBJS := $(subst .$(EXT),.o,$(CLIENTSOURCES))
SERVEROBJS := $(subst .$(EXT),.o,$(SERVERSOURCES))
LIBOBJS := $(subst .$(EXT),.o,$(LIBSOURCES))
COMMONOBJS := $(subst .$(EXT),.o,$(COMMONSOURCES))
MENUOBJS := $(subst .$(EXT),.o,$(MENUSOURCES))

all: mkdirs buildServer buildClient clean

server: mkdirs buildServer clean runServer

client: mkdirs buildClient clean runClient

menu: mkdirs buildMenu clean runMenu

buildClient: $(LIBOBJS) $(CLIENTOBJS) $(COMMONOBJS)
	@echo "\n  Linking $(CLIENTNAME)..."
	$(CC) -o $(OUTPUTDIR)/$(CLIENTNAME) $(LIBOBJS) $(COMMONOBJS) $(CLIENTOBJS) $(LDLIB) $(CFLAGS)
	@echo "\n"

buildServer: $(LIBOBJS) $(SERVEROBJS) $(COMMONOBJS)
	@echo "\n  Linking $(SERVERNAME)..."
	$(CC) -o $(OUTPUTDIR)/$(SERVERNAME) $(LIBOBJS) $(COMMONOBJS) $(SERVEROBJS) $(LDLIB) $(CFLAGS)
	@echo "\n"

buildMenu: $(LIBOBJS) $(MENUOBJS) $(COMMONOBJS)
	@echo "\n  Linking $(MENUNAME)..."
	$(CC) -o $(OUTPUTDIR)/$(MENUNAME) $(LIBOBJS) $(COMMONOBJS) $(MENUOBJS) $(LDLIB) $(CFLAGS)
	@echo "\n"

%.o : %.$(EXT)	
	$(CC) -c $< -o $@ $(INC) $(CFLAGS)

mkdirs:
	$(MK) $(OUTPUTDIR)

clean:
	@echo "  Cleaning..."
	$(RM) $(LIBOBJS) $(CLIENTOBJS) $(SERVEROBJS) $(COMMONOBJS)

runClient:
	@echo "\n  Starting to run $(CLIENTNAME)...\n"; ./$(OUTPUTDIR)/$(CLIENTNAME)

runServer:
	@echo "\n  Starting to run $(SERVERNAME)...\n"; ./$(OUTPUTDIR)/$(SERVERNAME)

runMenu:
	@echo "\n  Starting to run $(MENUNAME)...\n"; ./$(OUTPUTDIR)/$(MENUNAME) 

