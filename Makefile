misuring:
	mpicc gol_misuring.c -o gol_misuring
	gcc create_file.c -o create_file
	./create_file
	
	mpirun -np 1 gol_misuring 200 200 100 "strong_results_little.csv"
	mpirun -np 2 gol_misuring 200 200 100 "strong_results_little.csv"
	mpirun -np 3 gol_misuring 200 200 100 "strong_results_little.csv"
	mpirun -np 4 gol_misuring 200 200 100 "strong_results_little.csv"
	mpirun -np 5 gol_misuring 200 200 100 "strong_results_little.csv"
	mpirun -np 6 gol_misuring 200 200 100 "strong_results_little.csv"
	mpirun -np 7 gol_misuring 200 200 100 "strong_results_little.csv"
	mpirun -np 8 gol_misuring 200 200 100 "strong_results_little.csv"
	mpirun -np 9 gol_misuring 200 200 100 "strong_results_little.csv"
	mpirun -np 10 gol_misuring 200 200 100 "strong_results_little.csv"
	mpirun -np 11 gol_misuring 200 200 100 "strong_results_little.csv"
	mpirun -np 12 gol_misuring 200 200 100 "strong_results_little.csv"
	mpirun -np 13 gol_misuring 200 200 100 "strong_results_little.csv"
	mpirun -np 14 gol_misuring 200 200 100 "strong_results_little.csv"
	mpirun -np 15 gol_misuring 200 200 100 "strong_results_little.csv"
	mpirun -np 16 gol_misuring 200 200 100 "strong_results_little.csv"

	mpirun -np 1 gol_misuring 2000 2000 100 "strong_results_medium.csv"
	mpirun -np 2 gol_misuring 2000 2000 100 "strong_results_medium.csv"
	mpirun -np 3 gol_misuring 2000 2000 100 "strong_results_medium.csv"
	mpirun -np 4 gol_misuring 2000 2000 100 "strong_results_medium.csv"
	mpirun -np 5 gol_misuring 2000 2000 100 "strong_results_medium.csv"
	mpirun -np 6 gol_misuring 2000 2000 100 "strong_results_medium.csv"
	mpirun -np 7 gol_misuring 2000 2000 100 "strong_results_medium.csv"
	mpirun -np 8 gol_misuring 2000 2000 100 "strong_results_medium.csv"
	mpirun -np 9 gol_misuring 2000 2000 100 "strong_results_medium.csv"
	mpirun -np 10 gol_misuring 2000 2000 100 "strong_results_medium.csv"
	mpirun -np 11 gol_misuring 2000 2000 100 "strong_results_medium.csv"
	mpirun -np 12 gol_misuring 2000 2000 100 "strong_results_medium.csv"
	mpirun -np 13 gol_misuring 2000 2000 100 "strong_results_medium.csv"
	mpirun -np 14 gol_misuring 2000 2000 100 "strong_results_medium.csv"
	mpirun -np 15 gol_misuring 2000 2000 100 "strong_results_medium.csv"
	mpirun -np 16 gol_misuring 2000 2000 100 "strong_results_medium.csv"

	mpirun -np 1 gol_misuring 20000 20000 100 "strong_results_big.csv"
	mpirun -np 2 gol_misuring 20000 20000 100 "strong_results_big.csv"
	mpirun -np 3 gol_misuring 20000 20000 100 "strong_results_big.csv"
	mpirun -np 4 gol_misuring 20000 20000 100 "strong_results_big.csv"
	mpirun -np 5 gol_misuring 20000 20000 100 "strong_results_big.csv"
	mpirun -np 6 gol_misuring 20000 20000 100 "strong_results_big.csv"
	mpirun -np 7 gol_misuring 20000 20000 100 "strong_results_big.csv"
	mpirun -np 8 gol_misuring 20000 20000 100 "strong_results_big.csv"
	mpirun -np 9 gol_misuring 20000 20000 100 "strong_results_big.csv"
	mpirun -np 10 gol_misuring 20000 20000 100 "strong_results_big.csv"
	mpirun -np 11 gol_misuring 20000 20000 100 "strong_results_big.csv"
	mpirun -np 12 gol_misuring 20000 20000 100 "strong_results_big.csv"
	mpirun -np 13 gol_misuring 20000 20000 100 "strong_results_big.csv"
	mpirun -np 14 gol_misuring 20000 20000 100 "strong_results_big.csv"
	mpirun -np 15 gol_misuring 20000 20000 100 "strong_results_big.csv"
	mpirun -np 16 gol_misuring 20000 20000 100 "strong_results_big.csv"



	mpirun -np 1 gol_misuring 100 100 10 "weak_results_little.csv"
	mpirun -np 2 gol_misuring 200 100 10 "weak_results_little.csv"
	mpirun -np 3 gol_misuring 300 100 10 "weak_results_little.csv"
	mpirun -np 4 gol_misuring 400 100 10 "weak_results_little.csv"
	mpirun -np 5 gol_misuring 500 100 10 "weak_results_little.csv"
	mpirun -np 6 gol_misuring 600 100 10 "weak_results_little.csv"
	mpirun -np 7 gol_misuring 700 100 10 "weak_results_little.csv"
	mpirun -np 8 gol_misuring 800 100 10 "weak_results_little.csv"
	mpirun -np 9 gol_misuring 900 100 10 "weak_results_little.csv"
	mpirun -np 10 gol_misuring 1000 100 10 "weak_results_little.csv"
	mpirun -np 11 gol_misuring 1100 100 10 "weak_results_little.csv"
	mpirun -np 12 gol_misuring 1200 100 10 "weak_results_little.csv"
	mpirun -np 13 gol_misuring 1300 100 10 "weak_results_little.csv"
	mpirun -np 14 gol_misuring 1400 100 10 "weak_results_little.csv"
	mpirun -np 15 gol_misuring 1500 100 10 "weak_results_little.csv"
	mpirun -np 16 gol_misuring 1600 100 10 "weak_results_little.csv"

	mpirun -np 1 gol_misuring 1000 1000 10 "weak_results_medium.csv"
	mpirun -np 2 gol_misuring 2000 1000 10 "weak_results_medium.csv"
	mpirun -np 3 gol_misuring 3000 1000 10 "weak_results_medium.csv"
	mpirun -np 4 gol_misuring 4000 1000 10 "weak_results_medium.csv"
	mpirun -np 5 gol_misuring 5000 1000 10 "weak_results_medium.csv"
	mpirun -np 6 gol_misuring 6000 1000 10 "weak_results_medium.csv"
	mpirun -np 7 gol_misuring 7000 1000 10 "weak_results_medium.csv"
	mpirun -np 8 gol_misuring 8000 1000 10 "weak_results_medium.csv"
	mpirun -np 9 gol_misuring 9000 1000 10 "weak_results_medium.csv"
	mpirun -np 10 gol_misuring 10000 1000 10 "weak_results_medium.csv"
	mpirun -np 11 gol_misuring 11000 1000 10 "weak_results_medium.csv"
	mpirun -np 12 gol_misuring 12000 1000 10 "weak_results_medium.csv"
	mpirun -np 13 gol_misuring 13000 1000 10 "weak_results_medium.csv"
	mpirun -np 14 gol_misuring 14000 1000 10 "weak_results_medium.csv"
	mpirun -np 15 gol_misuring 15000 1000 10 "weak_results_medium.csv"
	mpirun -np 16 gol_misuring 16000 1000 10 "weak_results_medium.csv"

	mpirun -np 1 gol_misuring 2000 1000 10 "weak_results_big.csv"
	mpirun -np 2 gol_misuring 4000 1000 10 "weak_results_big.csv"
	mpirun -np 3 gol_misuring 6000 1000 10 "weak_results_big.csv"
	mpirun -np 4 gol_misuring 8000 1000 10 "weak_results_big.csv"
	mpirun -np 5 gol_misuring 10000 1000 10 "weak_results_big.csv"
	mpirun -np 6 gol_misuring 12000 1000 10 "weak_results_big.csv"
	mpirun -np 7 gol_misuring 14000 1000 10 "weak_results_big.csv"
	mpirun -np 8 gol_misuring 16000 1000 10 "weak_results_big.csv"
	mpirun -np 9 gol_misuring 18000 1000 10 "weak_results_big.csv"
	mpirun -np 10 gol_misuring 20000 1000 10 "weak_results_big.csv"
	mpirun -np 11 gol_misuring 22000 1000 10 "weak_results_big.csv"
	mpirun -np 12 gol_misuring 24000 1000 10 "weak_results_big.csv"
	mpirun -np 13 gol_misuring 26000 1000 10 "weak_results_big.csv"
	mpirun -np 14 gol_misuring 28000 1000 10 "weak_results_big.csv"
	mpirun -np 15 gol_misuring 30000 1000 10 "weak_results_big.csv"
	mpirun -np 16 gol_misuring 32000 1000 10 "weak_results_big.csv"


