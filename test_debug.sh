#!/bin/bash

echo "Testing with trace00.txt"
echo "Input:"
cat Test/trace00.txt
echo -e "\nMy program output:"
./code < Test/trace00.txt 2>&1
echo -e "\nDemo output:"
./Basic-Demo-64bit < Test/trace00.txt 2>&1