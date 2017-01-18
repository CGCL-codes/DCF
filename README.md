# The Dynamic Cuckoo Filter

## Overview
The Dynamic Cuckoo Filter (DCF) is an efficient approximate membership test data structure. It is especially designed for highly dynamic dataset. DCF supports extending and reducing its capacity by adding and removing building blocks. 

Through our experiment, DCF achieves 75% reduction in memory cost, 50% improvement in construction speed, and 80% improvement in speed of membership query compared with Dynamic Bloom Filter (DBF). Moreover, a DCF can support reliable item delete for a dynamic set and guarantee a stable false positive rate.

## API
Insert(item):

Query(item):

Delete(item):

Compact():

## How to use?
### OpenSSL libiary
Our implementation of DCF can be run in a Linux with OpenSSL libiary. See more details in https://www.openssl.org.
### Build example
Build the example:

make test

Run the example:

./test

### Configurations and Results
Configurations including false pisitive, item number and dataset path can be costomized in "configuration/config.txt". Change the value after the symbol "=" ONLY!!!

Results are show in "output/results.txt", including false positive, fingerprint size, building block number, operation time consumed and etc.


## Author and Copyright

Liangyi Liao (liaoliangyi@hust.edu.cn)

Hanhua Chen (chen@hust.edu.cn)

Hai Jin (hjin@hust.edu.cn)

Copyright (C) 2017, [STCS & CGCL](http://grid.hust.edu.cn/) and [Huazhong University of Science and Technology](http://www.hust.edu.cn).

## Acknowledgement


