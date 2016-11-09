# Final assignment for Operating Systems course at UNITN (2015-2016)

**Description:**

> RELAZIONE PROGETTO 2015-2016
Progetto di: Anas El Amraoui 173767 Giacomo Rossetto 171314 Luca Alberigo 165893

> Punti importanti:
E’ stata usata una fifo per la comunicazione tra processi.
Tutte le migliorie suggerite sono state completate.
Una miglioria supplementare è stata apportata per migliorare l’efficenza per quanto riguarda la memoria.

>Timeline:
L’esecuzione comincia innanzitutto con l’elaborazione dell’input e dei marcatori, nel caso in cui l’input sia inconsistente il programma esce subito dall’esecuzione. Nel caso in cui il programma abbia continuato la sua esecuzione, crea subito il file vuoto di output e inizializza la named pipe(FIFO). Viene letto una volta il file di input per calcolare le righe e la dimensione delle righe per creare un file temporaneo di offset. Dopodiché viene invocata la funzione split che effettua la ricerca binaria. La ricerca binaria per prima cosa controlla che il limite se settato, non sia stato superato. Poi crea un figlio che cerca verso a sinistra, e lo aspetta. Non è stato usato nessun tipo di vettore. la funzione splitsearch prende due indici e da in pasto al figlio la parte sinistra della fetta di ricerca, mentre in padre si occupa della fetta a destra. Ogni qualvolta si arriva alla base dell’albero di ricerca viene pescato il valore sommando n-righe del file degli offset. Poi viene aperto il file di input e mosso il cursore all’indirizzo della ennesima riga da leggere e vengono letti i byte della riga sempre in base a cosa c’è scritto sul file degli offset. In caso la comparazione del valore da cercare e il valore ripescato abbia esito positivo, viene scritto l’indice sul file output alla fine del file (append). Finita la ricerca viene rimosso il file degli offsets e scritte le statistiche della ricerca.


>Migliorie:
Sono state effettuate tutte le migliorie consigliate nella consegna del progetto quali:
Il programma gestisce gli argomenti in input con dei marcatori; -i per indicare il file di input , -o per indicare il file di output, -v per indicare il valore da cercare, gestisce sia dati numerici non interi, sia dati di tipo stringa.
Il programma permette di limitare la ricerca ai primi n risultati, tramite l’opzione di esecuzione -m (di default il programma esegue la ricerca su tutto l’input)
Se il programma non riesce a generare un processo figlio, il programma tramite la FIFO manda un codice di allarme (scrive “-1” nella FIFO) e fa terminare tutti i processi figlio, poi termina l’esecuzione.
