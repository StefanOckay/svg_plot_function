> diff.txt;
> output.txt;
> output_kontr.txt;
IFS='';
cat tests_multi.txt | while read LINE; do
	echo "$LINE" > line.txt;
	echo "exit" >> line.txt;
	/home/kontr/hw06/svggraph < line.txt &> f_output_kontr.txt;
	./svggraph < line.txt &> f_output.txt;
	echo "--------------" >> diff.txt;
	echo "function input: \"$LINE\"" >> diff.txt;
	if diff f_output.txt f_output_kontr.txt > /dev/null; then 
		echo "OK: the same output" >> diff.txt; 
	else 
		echo "NOT OK: different output" >> diff.txt;
	fi 
	echo "--------------" >> diff.txt;
	echo "--------------" >> output.txt;
        echo "--------------" >> output_kontr.txt;
	cat line.txt >> output.txt;
	cat line.txt >> output_kontr.txt;
	echo "--------------" >> output.txt;
	echo "--------------" >> output_kontr.txt;
	cat f_output.txt >> output.txt;
	cat f_output_kontr.txt >> output_kontr.txt;
done
IFS=' ';
