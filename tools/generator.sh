#!/bin/sh
make clean && make
chmod +x schar2int
cat url_externs | ./schar2int 1 | python gen_url_extern_code.py
mv url_extern_type_code.h ../include
mv url_extern_type_code.c ../sm
