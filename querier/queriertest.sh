### queriertest.sh - 'querier' module
### see TESTING.md for more information.
### Faustino Cortina – April 2018


echo "### Test with invalid arguments"
echo "'./querier' output:"
./querier
echo
echo "'./querier random_dir cs50_indexer_file' output:"
./querier random_dir cs50_indexer_file
echo
echo "'./querier cs50_crawler_output asdfasdf' output:"
./querier cs50_crawler_output asdfasdf
echo

echo "### Test using fuzztester on large index file created using cs50_wiki_crawler_output taken from the cs50 directory"
echo "'./fuzzquery wiki_index_file 10 0 | ./querier cs50_wiki_crawler_output/ wiki_index_file' output:"
./fuzzquery wiki_index_file 20 0 | ./querier cs50_wiki_crawler_output/ wiki_index_file
echo

echo "### Test using invalid queries"
echo "'./querier cs50_crawler_output cs50_index_file < testqueries2' output:"
./querier cs50_crawler_output cs50_index_file < testqueries2
echo


echo "### Compare with 'correct' results found in cs50 tse-output directory"
echo "### testqueries contains the same query searches that were done in the cs50_querier_output file"
echo "./querier cs50_crawler_output cs50_index_file < testqueries > queryresults"
./querier cs50_crawler_output cs50_index_file < testqueries > queryresults
echo
echo "'cat queryresults' output:"
cat queryresults
echo
echo "'cat cs50_querier_output' output:"
cat cs50_querier_output
