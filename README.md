Marching Squares

Algoritmul implementat urmareste enuntul problemei, citeste datele despre
imagini conform cerintei, pixelii si comenzile date ulterior sub forma
unor string-uri.
Algortimul este modularizat in functii pentru fiecare parte din cerinta:
alocare de memorie, eliberare, citire, creare grid, aplicare marching
squares. Matricele implicate in algoritm au fost alocate dinamic, din
cauza dimensiunilor mari (mai putin matricea de contur, care a fost
declarata pe stiva, intrucat are dimensiuni reduse: 16 x 16).
Functia main trateaza cazurile limita (comenzile de EXIT si READ), dar si
mesajele de eroare corespunzatoare. Restul comenzilor sunt interpretate
in functia read_commands. De asemenea, in functia read_commands se
initializeaza si matricea pentru contururi cu valorile corespunzatoare.
Fiecare comanda este interpretata separat, fiind alocata memorie in functie
de necesitate si elibereaza memoria la final. 
Matricea de pixeli este eliberata doar cand toate comenzile aplicate ei
s-au finalizat.
Functia resize transforma matricea de pixeli conform cerintei (de 4 ori mai
mare) si multiplica elemenetele existente.
Gridul este format in functia grid conform cerintei. Sunt tratate separat
cazurile limita (elementele care nu au 4 vecini), respectiv linia si coloana suplimentare.
Functia tresholding modifica gridul in functie de valoarea de referinta(200).
Functia bits realizeaza ultima parte a algoritmului de march. Calculeaza
valoarea fiecarui patrat in baza 10 si copiaza conturul corespunzator.
