#autori:
#Anas El Amraoui 173767
#Giacomo Rossetto 171314
#Luca Alberigo 165893
#Progetto: Spit-search multiprocesso

.PHONY: compile clean build test default dependencies
#compilatore
CC= gcc

#path dei files del progetto
MAIN= ./src/main.c

#file oggetto generati durante la compilazione
OBJ = ./build/main.o
SEARCH = ./build/splitsearch
LIB = ./src/lib/
ASSETS = ./assets/


#si lancia scrivendo makefile, mostra la lista dei comandi disponibili e le loro funzioni
default:
	@clear
	@echo "Progetto di Sistemi Operativi"
	@echo "I comandi disponibili sono:"
	@echo "<compile>  compila i sorgenti per creare il file eseguibile"
	@echo "<clean>  rimuove tutti i file generati durante la compilazione / esecuzione del programma"
	@echo "<build> esegue in successione i comandi clean e compile"
	@echo "<test> esegue una serie di test prefabbricati"


#compila i sorgenti (src) in eseguibili(build), deve dare il nome corretto al file
compile: dependecies

	@$(CC) -o $(SEARCH) $(OBJ) ./build/fifo_handler.o ./build/file_manager.o ./build/split_search.o ./build/input_parser.o ./build/printer.o
	@mkdir ./tmp

dependecies:
	@$(CC) -c -o ./build/printer.o $(LIB)printer.c
	@$(CC) -c -o ./build/fifo_handler.o $(LIB)fifo_handler.c
	@$(CC) -c -o ./build/file_manager.o $(LIB)file_manager.c
	@$(CC) -c -o ./build/split_search.o $(LIB)split_search.c
	@$(CC) -c -o ./build/input_parser.o $(LIB)input_parser.c
	@$(CC) -c -o $(OBJ) $(MAIN)


#finita
#pulisce i file di supporto generati durante la compilazione e l'esecuzione del progetto
clean:
	@rm -f ./build/*
	@rm -f *.txt
	@rm -rf ./tmp
	@rm -f myFIFO

#finita
#chiama clean e poi compile
build:
	@make -s clean
	@make -s compile

#chiama build e poi esegue il programma con dei casi di test
#casi di test: intero positivo, intero negativo, va con le stringhe e gli spazi, file enorme
# -m limite di ricerca -i imput(default imput.txt) -o output -v valore da cercare
test: build
	$(SEARCH) -i $(ASSETS)input1.txt -o $(ASSETS)output1.txt -v 1
	$(SEARCH) -i $(ASSETS)input2.txt -o $(ASSETS)output2.txt -v -9.51912242165
	$(SEARCH) -i $(ASSETS)input3.txt -o $(ASSETS)output3.txt -v "pera"
	$(SEARCH) -i $(ASSETS)input4.txt -o $(ASSETS)output4.txt -v 33
	$(SEARCH) -i $(ASSETS)input5.txt -o $(ASSETS)output5.txt -v "sistemi operativi"
