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
echo "vfork"
gcc vfork_use.c -o vfork_use.out
./vfork_use.out
echo "we can see that the global variable have been changed because the child proc run in parent's address zone"
echo "*************************************************************"
#gcc waitpid_use.c pr_exit.c -o waitpid_use.out
#./waitpid_use.out
#echo "*************************************************************"

rm *.out
