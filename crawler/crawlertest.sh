echo "### Creating empty directory 'data' for crawler to store webpages; if 'data' already exists, then everything in it is deleted ###"

mkdir data 2> /dev/null
rm -R data/* 2> /dev/null

echo
echo "### Test program with an invalid number of arguments ###"
echo

echo "# './crawler' output:"
./crawler
echo "# './crawler seedURL pageDirectory maxDepth fourthArgument' output:"
./crawler seedURL pageDirectory maxDepth fourthArgument
echo "# './crawler arg1 arg2' output:"
./crawler arg1 arg2
echo "# './crawler arg1' output:"
./crawler arg1

echo
echo "### Test crawler with invalid/un-useable arguments ###"
echo

echo "# './crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ data -1' output:"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ data -1
echo "# './crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ data 11' output:"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ data 11
echo "# './crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ randomDir 2' output:"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ randomDir 2
echo "# './crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ /bin 2' output:"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ /bin 2
echo "# './crawler http://google.com data 2' output:"
./crawler http://google.com data 2
echo "# './crawler http://old-www.cs.dartmouth.edu/asdfeivj data 2' output:"
./crawler http://old-www.cs.dartmouth.edu/asdfeivj data 2

echo
echo "### Test crawler on a simple set of pages that loop to see how crawler deals with duplicates and ensure crawler's output matches expectations"
echo

echo "# './crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ data 0' output:"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ data 0
echo && echo "# ls output for data directory should only output a single file with id '1'"
ls data
echo && echo "# this file should contain the html for 'http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/':"
cat data/1

echo && echo "# './crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ data 1' output:"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ data 1
echo && echo "# ls output for data directory should now be two files w/ names 1 & 2:"
ls data
echo && echo "# file 2 should contain the html for http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/A.html':"
cat data/2

echo && echo "# './crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ data 2' output:"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ data 2

echo && echo "# './crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ data 3' output:"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ data 3

echo && echo "# './crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ data 4' output:"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ data 4

echo && echo "# './crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ data 5' output:"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ data 5
echo && echo "there should now be 10 files in 'data' with names ranging from 1-10:"
ls data

echo
echo "### Clear data directory & test crawler on the same set of pages but starting at a different seed URL"
echo

echo "'./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/D.html data 3' output:"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/D.html data 3
echo && echo "'./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/D.html data 4' output:"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/D.html data 4

echo
echo "### Test crawler on the wikipedia playground on the Dartmouth CS page"
echo

echo "# './crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ data 0' output:"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ data 0
echo && echo "# './crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ data 1' output:"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ data 1
echo && echo "# './crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ data 2' output. Warning this output will be very long:"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ data 2
echo "# Number of files in 'data' folder (should be 1705):"
ls | wc -l
