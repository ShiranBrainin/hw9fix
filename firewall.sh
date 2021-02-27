#!/bin/bash

cat $1 >> temp.txt
echo >> temp.txt 
cat /dev/stdin >> temp2.txt
#cat temp2.txt #need to be deleted
touch output.txt final_output.txt

while read line; do
	#echo $line
	if [[ $line =~ ^([ ])*([#])+.*$ ]] ; then 
		#echo "if"
		continue;
		#else echo 0;
	elif [[ -z "$line" ]] ; then
		 #echo "else if"
		 continue;
	fi

	#echo "after if"
	line=$(echo $line | cut -d '#' -f1)
	#echo $line
	IFS=',' read -ra PATS <<< "$line"
	#echo ${PATS[0]}
	#echo ${PATS[1]}
	#echo ${PATS[2]}
	#echo ${PATS[3]}

	(cat temp2.txt | \
       ./firewall.exe "${PATS[0]}" 2>/dev/null| \
       ./firewall.exe "${PATS[1]}" 2>/dev/null| \
       ./firewall.exe "${PATS[2]}" 2>/dev/null| \
       ./firewall.exe "${PATS[3]}" 2>/dev/null >> output.txt)
done < temp.txt

#echo "output.txt:"
#cat output.txt
cat output.txt | sort | uniq >> mid_output.txt

while read line; do
	#echo "hi"
	echo $line | tr -d ' ' >> final_output.txt
done < mid_output.txt

cat final_output.txt
rm output.txt mid_output.txt final_output.txt temp.txt temp2.txt