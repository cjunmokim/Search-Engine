# Search-Engine

This is a miniature search engine like Google's, as per my project in the Software Implementation class at Dartmouth College.
The search engine is divided into 3 parts: Crawler, Indexer, and Query Engine.
Each of the three respective folders has its own Makefile and code that can be run,
but for ease follow the follwing steps to run the query engine, which combines all three parts.
Also, to avoid unnecessary complications where webservers detect and block the crawler, this search engine only works within the old-www.cs.dartmouth.edu domain.

1. cd ./query
2. make
3. ./query ../indexer/index.dat ../crawler/data

Now, the search engine is running, and it will prompt for a query line.
Type a query and press enter to get the list of websites with the query, ranked by frequency from high to low.
Note: Logical operators (AND, OR) are reserved for And and Or operations on the query words.
