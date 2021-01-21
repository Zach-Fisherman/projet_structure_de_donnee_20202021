test_table.exe : test_table.o p_table_des_matieres.o
	gcc -o test_table.exe test_table.o p_table_des_matieres.o

test_table.o : test_table.c
	gcc -c test_table.c

p_arbre_binaire.o : p_table_des_matieres.h p_table_des_matieres.c
	gcc -c p_table_des_matieres.c