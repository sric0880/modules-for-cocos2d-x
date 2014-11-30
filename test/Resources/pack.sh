#!/bin/sh
### encrypt all the json files from origin to the encrypt file folder
working_path=./config_raw
des_path=./config
json_path=$working_path/json
josn_raw_path=$working_path/json_raw/
py_tool_path=../libs/tools/trans_excel/excel_to_dict.py
key=123456789012345

for file in `ls $working_path`
do
	file_path="$working_path/$file"
	echo $file_path
	if [ -d $file_path ];then
		echo "$file_path is not a file"
	else
		echo "python $py_tool_path $file_path json"
		python $py_tool_path $file_path json
	fi
done
if [ ! -d $json_path ];then
	echo "mkdir $json_path"
	mkdir $json_path
fi
for json_file in `ls . | grep json`
do
	mv "$json_file" $json_path
done
if [ ! -d $josn_raw_path ];then
	echo "mkdir $josn_raw_path"
	mkdir $josn_raw_path
fi
cp $json_path/* $josn_raw_path
../libs/tools/aes/aes -e $json_path $key
cp $json_path/* $des_path