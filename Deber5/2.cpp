/*
 * Exercise 2 - Analysis of Algorithms Lab 05
 * Binary Search vs Interpolation Search on 10,000,000 random integers
 *
 * Both algorithms require a SORTED array as a precondition.
 *
 * Binary Search:      O(log n) guaranteed
 * Interpolation Search: O(log log n) average for uniform data,
 *                       O(n) worst case (non-uniform / adversarial data)
 */

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

using namespace std;
using namespace chrono;

/* ------------------------------------------------------------------ */
/* Binary Search                                                        */
/* Returns index of key in sorted arr, or -1 if not found.             */
/* ------------------------------------------------------------------ */
int binarySearch(const vector<int>& arr, int key) {
    int left  = 0;
    int right = static_cast<int>(arr.size()) - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;   /* avoids overflow */

        if (arr[mid] == key) return mid;
        if (arr[mid] <  key) left  = mid + 1;
        else                  right = mid - 1;
    }
    return -1;
}

/* ------------------------------------------------------------------ */
/* Interpolation Search                                                 */
/* Estimates the position using a linear interpolation formula:        */
/*   pos = low + (key - arr[low]) * (high - low) / (arr[high]-arr[low])*/
/* ------------------------------------------------------------------ */
int interpolationSearch(const vector<int>& arr, int key) {
    int low  = 0;
    int high = static_cast<int>(arr.size()) - 1;

    while (low <= high && key >= arr[low] && key <= arr[high]) {

        if (arr[low] == arr[high]) {            /* all elements equal */
            return (arr[low] == key) ? low : -1;
        }

        /* cast to long long to prevent 32-bit overflow */
        long long range   = static_cast<long long>(arr[high]) - arr[low];
        long long offset  = static_cast<long long>(key        - arr[low]);
        int pos = low + static_cast<int>(offset * (high - low) / range);

        if (arr[pos] == key) return pos;
        if (arr[pos] <  key) low  = pos + 1;
        else                  high = pos - 1;
    }
    return -1;
}

/* ------------------------------------------------------------------ */
/* Benchmark helper: runs a search function 5 times and returns the   */
/* median elapsed time in nanoseconds to reduce clock noise.           */
/* ------------------------------------------------------------------ */
template <typename SearchFn>
long long benchmark(SearchFn fn, const vector<int>& arr, int key,
                    int& result_out, int runs = 5) {
    vector<long long> times(runs);
    for (int r = 0; r < runs; r++) {
        auto t0   = high_resolution_clock::now();
        result_out = fn(arr, key);
        auto t1   = high_resolution_clock::now();
        times[r]  = duration_cast<nanoseconds>(t1 - t0).count();
    }
    sort(times.begin(), times.end());
    return times[runs / 2];   /* median */
}

/* ------------------------------------------------------------------ */
/* Main                                                                 */
/* ------------------------------------------------------------------ */
int main() {
    const int    SIZE  = 10'000'000;
    const int    RANGE = 1'000'000'000;

    /* ---------- generate random data -------------------------------- */
    cout << "Generating " << SIZE << " random integers in [1, "
         << RANGE << "]...\n";

    random_device              rd;
    mt19937                    gen(rd());
    uniform_int_distribution<int> dist(1, RANGE);

    vector<int> numbers(SIZE);
    for (int i = 0; i < SIZE; i++)
        numbers[i] = dist(gen);

    /* ---------- sort ------------------------------------------------ */
    cout << "Sorting... ";
    auto t_sort_start = high_resolution_clock::now();
    sort(numbers.begin(), numbers.end());
    auto t_sort_end   = high_resolution_clock::now();
    double sortMs = duration_cast<nanoseconds>(t_sort_end - t_sort_start)
                        .count() / 1e6;
    cout << fixed << setprecision(2) << sortMs << " ms\n";

    /* ---------- choose search keys ---------------------------------- */
    /* Key guaranteed to EXIST (middle element) */
    int keyPresent = numbers[SIZE / 2];
    /* Key likely NOT to exist */
    int keyAbsent  = 0;   /* 0 is below our range [1..1e9] */

    /* ---------- run benchmarks -------------------------------------- */
    int bResult, iResult;

    long long bTimePresent = benchmark(binarySearch,        numbers, keyPresent, bResult);
    long long iTimePresent = benchmark(interpolationSearch, numbers, keyPresent, iResult);

    int bResultA, iResultA;
    long long bTimeAbsent  = benchmark(binarySearch,        numbers, keyAbsent, bResultA);
    long long iTimeAbsent  = benchmark(interpolationSearch, numbers, keyAbsent, iResultA);

    /* ---------- report ---------------------------------------------- */
    const int W = 28;
    cout << "\n";
    cout << "======================================================\n";
    cout << "  RESULTS  (n = " << SIZE << ")\n";
    cout << "======================================================\n";

    auto printRow = [&](const string& label, long long ns, int idx) {
        cout << left  << setw(W) << label
             << right << setw(10) << ns       << " ns"
             << "   (" << fixed << setprecision(4) << ns / 1e6 << " ms)"
             << "   index=" << idx << "\n";
    };

    cout << "\n--- Key EXISTS (value = " << keyPresent << ") ---\n";
    printRow("Binary Search:",        bTimePresent, bResult);
    printRow("Interpolation Search:", iTimePresent, iResult);

    cout << "\n--- Key ABSENT (value = " << keyAbsent << ") ---\n";
    printRow("Binary Search:",        bTimeAbsent,  bResultA);
    printRow("Interpolation Search:", iTimeAbsent,  iResultA);

    /* ---------- theoretical summary --------------------------------- */
    cout << "\n--- Theoretical complexity (n = " << SIZE << ") ---\n";
    cout << "  Binary Search      O(log n)      => ~"
         << static_cast<int>(log2(SIZE) + 1) << " comparisons max\n";
    cout << "  Interpolation Srch O(log log n)  => ~"
         << static_cast<int>(log2(log2(SIZE)) + 1)
         << " comparisons avg (uniform data)\n";

    return 0;
}