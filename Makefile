all: bsgs ic pollardrho

bsgs: bsgs.cpp
	gcc bsgs.cpp -o bsgs -lgmp -lstdc++ -std=gnu++11

ic: ic.cpp
	gcc ic.cpp -o ic -lgmp -lstdc++ -std=gnu++11 -lflint -lmpfr -I /usr/local/include/flint

pollardrho: pollardrho.cpp
	gcc pollardrho.cpp -o pollardrho

clean: 
	  $(RM) bsgs ic pollardrho
