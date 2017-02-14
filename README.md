# The Dynamic Cuckoo Filter

## Overview
The Dynamic Cuckoo Filter (DCF) is an efficient approximate membership test data structure. Simarlar with the classic Bloom filter, it aims at quickly determining whether or not an item belongs to a given dataset with an acceptable error. Different from Bloom filter and its variants, DCF is especially designed for highly dynamic dataset and support extending and reducing its capacity by adding and removing building blocks. The advantages of DCF is as follow

*the DCF design is the first to achieve both reliable item deletion and flexibly extending/reducing for approximate set representation and membership testing
