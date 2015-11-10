#!/bin/bash

# Tests genéricos de 1 a 10 clientes
for i in {1..10}
do
    echo "generico $(($i * 2))"
    mpiexec -np $(($i * 2)) ./tp3 > /dev/null 2> "tests/test$i.tst"
done

# más tests
mpiexec -np 4 ./tp3 a 2 100 100 b 4 50 50 > /dev/null 2> "tests/test11.tst"
mpiexec -np 6 ./tp3 a 10 30 30 b 3 50 50 c 4 200 200 > /dev/null 2> "tests/test12.tst"
mpiexec -np 8 ./tp3 a 2 40 20 b 3 60 70 c 4 30 60 d 1 60 1000 > /dev/null 2> "tests/test13.tst"
mpiexec -np 10 ./tp3 a 2 10000 1000 b 3 0 0 c 4 0 0 d 1 0 0 e 5 0 0 > /dev/null 2> "tests/test14.tst"
mpiexec -np 14 ./tp3 a 2 50 100 b 3 30 400 c 4 10 10 d 1 1000 5000 e 2 10 0 f 3 0 0 g 4 20 0 h 1 0 0 > /dev/null 2> "tests/test15.tst"
mpiexec -np 24 ./tp3 a 2 0 0 b 3 100 150 c 4 500 1000 d 1 0 0 e 2 0 0 h 3 0 0 i 4 0 0 j 1 0 0 k 2 0 0 l 3 0 0 m 4 0 0 n 1 0 0 o 2 1000 500 p 3 0 0 q 4 0 0 r 10 20 50 > /dev/null 2> "tests/test16.tst"

# Otros tests manuales
echo "enunciado"
mpiexec -np 6 ./tp3 @ 4 0 1000 b 9 250 0 c 9 500 0 > /dev/null 2> "tests/enunciado.tst"

echo "sin letras"
mpiexec -np 8 ./tp3 ^ 10 80 20 _ 0 50 100 [ 7 340 2 ] 20 100 343 > /dev/null 2> "tests/sinletras.tst"

echo "carrera"
mpiexec -np 12 ./tp3 R 1 0 0 a 1 0 0 c 1 0 0 i 1 0 0 n 1 0 0 g 1 0 0 > /dev/null 2> "tests/carrera.tst"

echo "solo"
mpiexec -np 2 ./tp3 B 7 135 12 > /dev/null 2> "tests/solo.tst"
