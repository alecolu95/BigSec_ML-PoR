default:
	gcc -o ml_por *.c -L. -lpbc -lgmp -lssl -lcrypto
do_build:
	gcc data_owner.c -L. -lpbc -lgmp
s_build:
	gcc server.c -L. -lpbc -lgmp
run:
	./ml_por < a.param
