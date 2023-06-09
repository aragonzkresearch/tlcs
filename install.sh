#!/bin/bash
ldconfig -p |grep libcrypto 1>/dev/null
if [ $? == 1 ]; then
	echo "libcrypto is required to install this package but was not found."
	exit 1
fi
if [ ! -f mcl/lib/libmclbn384_256.a ] || [ ! -f mcl/lib/libmcl.a ]; then
    echo "mcl library is required but was not found."
echo "Run the following commands:"
echo "git clone https://github.com/herumi/mcl.git"
echo "cd mcl"
echo "make all"
echo "cd .."
exit 1
fi
mkdir bin
mkdir lib
make all
echo "Installation terminated."
echo "Now execute the following commands:"
echo "cp ./lib/libtlcs.so /usr/lib"
echo "cp ./lib/libtlcs_rsa.so /usr/lib"
echo "cp ./lib/tlcs_bls_g1.so /usr/lib"
echo "cp ./lib/tlcs_ss.so /usr/lib"
echo "sudo ldconfig"
