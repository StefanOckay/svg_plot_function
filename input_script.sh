FILE=tests_multi.txt
> output.txt;
IFS='';
cat $FILE | while read LINE; do
	echo "$LINE" > line.txt;
	echo "exit" >> line.txt;
	echo "function input: \"$LINE\"" >> output.txt;
	./svggraph < line.txt &> f_output.txt;
	echo "output:" >> output.txt;
	cat f_output.txt >> output.txt;
	echo "-------END-------" >> output.txt;
done
IFS=' ';
