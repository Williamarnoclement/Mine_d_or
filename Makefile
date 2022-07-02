main : main.o etat_admin.o  etat_compt.o etat_mineur.o helper.o usual.o
	gcc -o main main.o etat_admin.o  etat_mineur.o etat_compt.o helper.o usual.o

main.o : main.c
	gcc -c main.c

administrateur : administrateur.o usual.o helper.o
	gcc -o administrateur administrateur.o usual.o helper.o

mineur : mineur.o usual.o helper.o etat_mineur.o
	gcc -o mineur mineur.o usual.o helper.o etat_mineur.o

comptable : comptable.o usual.o helper.o etat_compt.o etat_mineur.o
	gcc -o comptable comptable.o usual.o helper.o etat_compt.o etat_mineur.o

administrateur.o : administrateur.c
	gcc -c administrateur.c

comptable.o : comptable.c
	gcc -c comptable.c

mineur.o : mineur.c
	gcc -c mineur.c

etat_admin.o : etat_admin.c
	gcc -c etat_admin.c

etat_compt.o : etat_compt.c
	gcc -c etat_compt.c

etat_mineur.o : etat_mineur.c
	gcc -c etat_mineur.c

helper.o : helper.c
	gcc -c helper.c

usual.o : usual.c
	gcc -c usual.c

clean :
	rm *.o main administrateur mineur comptable

all : main administrateur mineur comptable
