#!/bin/sh
### encrypt all the json files from origin to the encrypt file folder
key=123456789012345
origin_path=./origin
encrypt_path=./encrypt
aes_executable=../../module_data_manager/aes/build/Default/aes

if [ ! -d $encrypt_path ];then
	echo "create $encrypt_path"
	mkdir $encrypt_path
fi

cp $origin_path/* $encrypt_path
$aes_executable -e $encrypt_path $key