echo "run_all"
echo "*************************************************************"
echo "fork"
g++ fork_use.c -o fork_use.out
./fork_use.out
echo "we can see that the global variable is not changed even if it changed on child proc"
echo "- - - - - - - - - - - - - - - - - - "
echo "redirection,change stdout to temp.out"
./fork_use.out > temp.out
cat temp.out
echo "*************************************************************"
