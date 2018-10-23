# TESTING ARGUMENTS PASSED TO 'INDEXER' FOR VALIDITY
echo "Testing indexer program with invalid parameters:"
echo "'./indexer onlyoneargument' output:"
./indexer onlyoneargument
echo
echo "'./indexer one two threeargs' output:"
./indexer one two threeargs
echo
echo "'./indexer ../ file' output:"
./indexer ../ file
echo
echo "'./indexer crawler-output-small crawler-output-small' output:"
./indexer crawler-output-small/ crawler-output-small/
echo
# TESTING 'INDEXER' PROGRAM
echo "Indexing 'crawler-output-small' file, which contains the crawler output of 7 webpages, and storing the indexer file under the name 'file'..."
./indexer crawler-output-small file
echo
echo "Indexer program complete"
echo
echo "First 10 lines of 'file':"
head file
echo
# COMPARING 'INDEXER' OUTPUT WITH 'CORRECT' OUTPUT PROVIDED BY THE CS50 DEPARTMENT
echo "Sorting file and cs50_indexer_file, which is the ouput file for crawler-output-small provided in the cs50 directory, so they are in the same order..."
gawk -f indexsort.awk file > file.sorted
gawk -f indexsort.awk cs50_indexer_file > cs50_indexer_file.sorted
echo
echo "Sort complete"
echo
echo "Comparing 'file' with 'cs50_indexer_file'..."
echo "'diff --brief file cs50_indexer_file' output (should have no output):"
diff --brief file.sorted cs50_indexer_file.sorted
echo "–––End of diff output–––"
echo
# TESTING ARGUMENTS PASSED TO 'INDEXTEST' FOR VALIDITY
echo "Testing indextest program with invalid parameters:"
echo "(indextester reads and builds an indexer from the old file and writes it to a new file, so the two files should have the same content)"
echo
echo "'./indextest onlyoneargument' output:"
./indextest onlyoneargument
echo
echo "'./indextest one two threeargs' output:"
./indextest one two threeargs
echo
echo "'./indextest invalidfile file2' output:"
./indextest invalidfile file
echo
echo "'./indextest file crawler-output-small' output:"
./indextest file crawler-output-small/
echo
# TESTING 'INDEXTEST' PROGRAM
echo "Running indextest with 'file' as the old file and 'file2' as the new file..."
./indextest file file2
echo
echo "indextest program complete"
echo
# COMPARING 'INDEXER' OUTPUT WITH 'INDEXTEST' OUTPUT
echo "Sorting 'file2' so its in the same order as 'file'..."
gawk -f indexsort.awk file2 > file2.sorted
echo "Comparing 'file' with 'file2'..."
echo "'diff --brief file.sorted file2.sorted' output (should have no output):"
diff --brief file.sorted file2.sorted
echo "–––End of diff output–––"
echo
