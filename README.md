**Project 1**

./allocate -p 1 -f testcases/task1/input/test_p1_n_1.txt | diff - testcases/task1/output/test_p1_n_1.out
./allocate -p 1 -f testcases/task1/input/test_p1_n_2.txt | diff - testcases/task1/output/test_p1_n_2.out
./allocate -p 2 -f testcases/task2/input/test_p2_n_1.txt | diff - testcases/task2/output/test_p2_n_1.out
./allocate -p 2 -f testcases/task2/input/test_p2_n_2.txt | diff - testcases/task2/output/test_p2_n_2.out
./allocate -p 2 -f testcases/task3/input/test_p2_p_1.txt | diff - testcases/task3/output/test_p2_p_1.out
./allocate -p 2 -f testcases/task3/input/test_p2_p_2.txt | diff - testcases/task3/output/test_p2_p_2.out
./allocate -p 4 -f testcases/task4/input/test_p4_n_1.txt | diff - testcases/task4/output/test_p4_n_1.out
./allocate -p 4 -f testcases/task4/input/test_p4_n_2.txt | diff - testcases/task4/output/test_p4_n_2.out
./allocate -p 4 -f testcases/task5/input/test_p4_p_1.txt | diff - testcases/task5/output/test_p4_p_1.out
./allocate -p 4 -f testcases/task5/input/test_p4_p_2.txt | diff - testcases/task5/output/test_p4_p_2.out
./allocate -p 1 -f testcases/task6/input/test_p1_n_1.txt | diff - testcases/task6/output/test_p1_n_1.out
./allocate -p 4 -f testcases/task6/input/test_p4_p_2.txt | diff - testcases/task6/output/test_p4_p_2.out
./allocate -p 2 -f testcases/task7/test_chal_p2_n.txt -c
./allocate -p 2 -f testcases/task7/test_chal_p2_p.txt -c
./allocate -p 3 -f testcases/task7/test_chal_p3_p.txt -c
./allocate -p 4 -f testcases/task7/test_chal_p4_n.txt -c
./allocate -p 4 -f testcases/task7/test_chal_p4_p.txt -c
./allocate -p 5 -f testcases/task7/test_chal_p5_n.txt -c
./allocate -p 5 -f testcases/task7/test_chal_p5_p.txt -c
./allocate -p 6 -f testcases/task7/test_chal_p6_n_equal.txt -c
./allocate -p 6 -f testcases/task7/test_chal_p6_p_equal.txt -c


valgrind ./allocate -p 1 -f testcases/task1/input/test_p1_n_1.txt | diff - testcases/task1/output/test_p1_n_1.out
valgrind ./allocate -p 1 -f testcases/task1/input/test_p1_n_2.txt | diff - testcases/task1/output/test_p1_n_2.out
valgrind ./allocate -p 2 -f testcases/task2/input/test_p2_n_1.txt | diff - testcases/task2/output/test_p2_n_1.out
valgrind ./allocate -p 2 -f testcases/task2/input/test_p2_n_2.txt | diff - testcases/task2/output/test_p2_n_2.out
valgrind ./allocate -p 2 -f testcases/task3/input/test_p2_p_1.txt | diff - testcases/task3/output/test_p2_p_1.out
valgrind ./allocate -p 2 -f testcases/task3/input/test_p2_p_2.txt | diff - testcases/task3/output/test_p2_p_2.out
valgrind ./allocate -p 4 -f testcases/task4/input/test_p4_n_1.txt | diff - testcases/task4/output/test_p4_n_1.out
valgrind ./allocate -p 4 -f testcases/task4/input/test_p4_n_2.txt | diff - testcases/task4/output/test_p4_n_2.out
valgrind ./allocate -p 4 -f testcases/task5/input/test_p4_p_1.txt | diff - testcases/task5/output/test_p4_p_1.out
valgrind ./allocate -p 4 -f testcases/task5/input/test_p4_p_2.txt | diff - testcases/task5/output/test_p4_p_2.out
valgrind ./allocate -p 1 -f testcases/task6/input/test_p1_n_1.txt | diff - testcases/task6/output/test_p1_n_1.out
valgrind ./allocate -p 4 -f testcases/task6/input/test_p4_p_2.txt | diff - testcases/task6/output/test_p4_p_2.out
valgrind ./allocate -p 2 -f testcases/task7/test_chal_p2_n.txt -c
valgrind ./allocate -p 2 -f testcases/task7/test_chal_p2_p.txt -c
valgrind ./allocate -p 3 -f testcases/task7/test_chal_p3_p.txt -c
valgrind ./allocate -p 4 -f testcases/task7/test_chal_p4_n.txt -c
valgrind ./allocate -p 4 -f testcases/task7/test_chal_p4_p.txt -c
valgrind ./allocate -p 5 -f testcases/task7/test_chal_p5_n.txt -c
valgrind ./allocate -p 5 -f testcases/task7/test_chal_p5_p.txt -c
valgrind ./allocate -p 6 -f testcases/task7/test_chal_p6_n_equal.txt -c
valgrind ./allocate -p 6 -f testcases/task7/test_chal_p6_p_equal.txt -c
