Steven Comer
CSC 718
Project Part II - README
25 Oct 2016

Files included in submission:
	merge.c
	Makefile
	comer_project_part2_data.xlsx
	comer_project_part2_documentation.docx
	README.txt

Compile with:
	gcc -o merge -fopenmp merge.c

Run with:
	./merge <dynamic_flag> <N>
		*dynamic_flag can be 0 or 1, 0 gives more control of thread count
		*I used a setting of 0 for my testing

Output of "./merge 0 10000"
	Sequential execution time:		      1.69 ms
	2 threads execution time:		      0.10 ms
	4 threads execution time:		     11.79 ms
