#!/bin/bash

# Tests genéricos de 1 a 10 clientes
for i in {1..10}
do
    echo "generico $(($i * 2))"
    mpiexec -np $(($i * 2)) ./tp3 > /dev/null 2> "tests/test$i.tst"
done

# Otros tests manuales
echo "enunciado"
mpiexec -np 6 ./tp3 @ 4 0 1000 b 9 250 0 c 9 500 0 > /dev/null 2> "tests/enunciado.tst"

echo "sin letras"
mpiexec -np 8 ./tp3 ^ 10 80 20 _ 0 50 100 [ 7 340 2 ] 20 100 343 > /dev/null 2> "tests/sinletras.tst"

echo "carrera"
mpiexec -np 12 ./tp3 R 1 0 0 a 1 0 0 c 1 0 0 i 1 0 0 n 1 0 0 g 1 0 0 > /dev/null 2> "tests/carrera.tst"

echo "solo"
mpiexec -np 2 ./tp3 B 7 135 12 > /dev/null 2> "tests/solo.tst"
