CC=gcc
CFLAGS=-Wall -std=c99 -fopenmp
MERGESORT=merge_sort
BINARYSEARCH=binary_search
MERGESORTSRC=src/sorting/parallel_merge_sort/merge_sort.c
BINARYSEARCHSRC=src/search/parallel_binary_search/binary_search.c
MERGESORTINPUTS=src/sorting/parallel_merge_sort/inputs
BINARYSEARCHINPUTS=src/search/parallel_binary_search/inputs

mergesort:
	$(CC) $(CFLAGS) -o $(MERGESORT) $(MERGESORTSRC)
	./$(MERGESORT) $(MERGESORTINPUTS)/small_input.txt 1
	./$(MERGESORT) $(MERGESORTINPUTS)/small_input.txt 2
	./$(MERGESORT) $(MERGESORTINPUTS)/small_input.txt 4
	./$(MERGESORT) $(MERGESORTINPUTS)/small_input.txt 8

	./$(MERGESORT) $(MERGESORTINPUTS)/medium_input.txt 1
	./$(MERGESORT) $(MERGESORTINPUTS)/medium_input.txt 2
	./$(MERGESORT) $(MERGESORTINPUTS)/medium_input.txt 4
	./$(MERGESORT) $(MERGESORTINPUTS)/medium_input.txt 8

	./$(MERGESORT) $(MERGESORTINPUTS)/large_input.txt 1
	./$(MERGESORT) $(MERGESORTINPUTS)/large_input.txt 2
	./$(MERGESORT) $(MERGESORTINPUTS)/large_input.txt 4
	./$(MERGESORT) $(MERGESORTINPUTS)/large_input.txt 8

	./$(MERGESORT) $(MERGESORTINPUTS)/extreme_large_input.txt 1
	./$(MERGESORT) $(MERGESORTINPUTS)/extreme_large_input.txt 2
	./$(MERGESORT) $(MERGESORTINPUTS)/extreme_large_input.txt 4
	./$(MERGESORT) $(MERGESORTINPUTS)/extreme_large_input.txt 8
binarysearch:
	$(CC) $(CFLAGS) -o $(BINARYSEARCH) $(BINARYSEARCHSRC)
	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/small_input.txt 1 50
	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/small_input.txt 2 50
	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/small_input.txt 4 50
	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/small_input.txt 8 50

	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/medium_input.txt 1 50
	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/medium_input.txt 2 50
	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/medium_input.txt 4 50
	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/medium_input.txt 8 50

	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/large_input.txt 1 50
	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/large_input.txt 2 50
	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/large_input.txt 4 50
	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/large_input.txt 8 50

	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/extreme_large_input.txt 1 50
	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/extreme_large_input.txt 2 50
	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/extreme_large_input.txt 4 50
	./$(BINARYSEARCH) $(BINARYSEARCHINPUTS)/extreme_large_input.txt 8 50
clean:
	rm -f $(MERGESORT) $(BINARYSEARCH)
