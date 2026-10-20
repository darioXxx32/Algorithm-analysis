g++ -O2 -std=c++17 src\pagerank.cpp -o pagerank.exe
g++ -O2 -std=c++17 src\generate_dataset.cpp -o generate_dataset.exe

.\generate_dataset.exe --output data\synthetic_100.csv --nodes 100 --avg-degree 6 --seed 2026
.\generate_dataset.exe --output data\synthetic_500.csv --nodes 500 --avg-degree 6 --seed 2027
.\generate_dataset.exe --output data\synthetic_1000.csv --nodes 1000 --avg-degree 6 --seed 2028
.\generate_dataset.exe --output data\synthetic_5000.csv --nodes 5000 --avg-degree 6 --seed 2029

.\pagerank.exe --input data\web_example.csv --output results\web_example_scores.csv --damping 0.85 --tol 1e-10 --max-iter 300
.\pagerank.exe --input data\synthetic_100.csv --output results\synthetic_100_scores.csv --damping 0.85 --tol 1e-10 --max-iter 300
.\pagerank.exe --input data\synthetic_500.csv --output results\synthetic_500_scores.csv --damping 0.85 --tol 1e-10 --max-iter 300
.\pagerank.exe --input data\synthetic_1000.csv --output results\synthetic_1000_scores.csv --damping 0.85 --tol 1e-10 --max-iter 300
.\pagerank.exe --input data\synthetic_5000.csv --output results\synthetic_5000_scores.csv --damping 0.85 --tol 1e-10 --max-iter 300

.\pagerank.exe --input data\synthetic_1000.csv --output results\synthetic_1000_d050.csv --damping 0.50 --tol 1e-10 --max-iter 300
.\pagerank.exe --input data\synthetic_1000.csv --output results\synthetic_1000_d075.csv --damping 0.75 --tol 1e-10 --max-iter 300
.\pagerank.exe --input data\synthetic_1000.csv --output results\synthetic_1000_d090.csv --damping 0.90 --tol 1e-10 --max-iter 300
.\pagerank.exe --input data\synthetic_1000.csv --output results\synthetic_1000_d095.csv --damping 0.95 --tol 1e-10 --max-iter 300
