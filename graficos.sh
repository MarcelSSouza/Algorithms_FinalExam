#!/bin/bash
s

for i in {10000..100000..100}
do
	./multi_ordered_tree 101043 $i
done

for i in {100000..1000000..10000}
do
	./multi_ordered_tree 101043 $i
done

