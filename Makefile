#autori:
#Anas El Amraoui 173767
#Giacomo Rossetto 171314
#Luca Alberigo 165893
#Progetto: search distribuito

.PHONY: compile clean build test default dependencies
#compilatore
CC= gcc

#path dei files del progetto
MAIN= ./src/main.c


#file oggetto generati durante la compilazione
OBJ= ./build/main.o
SEARCH= ./build/splitsearch
LIB= ./src/lib/



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
	$(CC) -o $(SEARCH) $(OBJ) ./build/input_parser.o ./build/fifo_handler.o ./build/file_manager.o ./build/split_search.o
	mkdir ./tmp


dependecies:
	$(CC) -c -o ./build/fifo_handler.o $(LIB)fifo_handler.c
	$(CC) -c -o ./build/input_parser.o $(LIB)input_parser.c
	$(CC) -c -o ./build/file_manager.o $(LIB)file_manager.c
	$(CC) -c -o ./build/split_search.o $(LIB)split_search.c
	$(CC) -c -o $(OBJ) $(MAIN)


#finita
#pulisce i file di supporto generati durante la compilazione e l'esecuzione del progetto
clean:
	rm -f ./build/*
	rm -f *.txt
	rm -rf ./tmp
	rm -f myFIFO

#finita
#chiama clean e poi compile
build:
	@make clean
	@make compile

#chiama build e poi esegue il programma con dei casi di test
#casi di test: intero positivo, intero negativo, va con le stringhe e gli spazi, file enorme
# -m limite di ricerca -i imput(default imput.txt) -o output -v valore da cercare
test: compile
	./$(SEARCH) -i input1.txt -o output1.txt -v
	./$(SEARCH) -i input2.txt -o output2.txt -v
	./$(SEARCH) -i input3.txt -o output3.txt -v
	./$(SEARCH) -i input4.txt -o output4.txt -v
