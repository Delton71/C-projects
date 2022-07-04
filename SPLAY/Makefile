FLAGS=-Wall -g
RESOURCES=hashtable.c main.c
EXECUTABLE=main

run_main: $(F).c $(TXT).txt
	gcc $(FLAGS) $(F).c -o $(F) -lm
	./$(F) < $(TXT).txt

run: $(TXT).txt
	gcc $(FLAGS) $(RESOURCES) -o $(EXECUTABLE) -lm
	./$(EXECUTABLE) < $(TXT).txt
