find src -name '*.c' -o -name '*.cc' -exec wc -l {} + | tail -n1
find include -name '*.h' -exec wc -l {} + | tail -n1
