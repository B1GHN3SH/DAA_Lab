<h1 align="center">Design and Analysis of Algorithms</h1>

<p align="center">
  <strong>DAA Laboratory Assignments — IIIT Bhubaneswar</strong>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Language-C-A8B9CC?style=flat-square&logo=c&logoColor=white" alt="C" />
  <img src="https://img.shields.io/badge/Compiler-GCC-FE7A16?style=flat-square&logo=gnu&logoColor=white" alt="GCC" />
  <img src="https://img.shields.io/badge/Analysis-Asymptotic%20%2B%20Empirical-6E4AFF?style=flat-square" alt="Analysis" />
  <img src="https://img.shields.io/badge/Plots-CSV%20%2B%20PNG-1D9E5E?style=flat-square" alt="Plots" />
  <img src="https://img.shields.io/badge/Platform-WSL%20%7C%20Windows-4EAA25?style=flat-square&logo=linux&logoColor=white" alt="Platform" />
</p>

---

## Introduction

This repository holds my solutions for the Design and Analysis of Algorithms
Laboratory at IIIT Bhubaneswar. Every lab session gets its own folder, and
inside that, every question gets its own subfolder holding the source file
for that question plus anything it generates (CSV data, PNG plots).

Most of these questions ask for more than "does it run" — they ask what
happens as `n` grows. So the programs here do two jobs: solve the problem,
and instrument it — counting comparisons, counting moves, tabulating values
across a range of `n` — then export that data as CSV so the growth can be
plotted rather than just asserted.

---

## Student Information

| Field      | Details                                      |
| ---------- | -------------------------------------------- |
| Name       | Bighnesh Binoy                               |
| Student ID | B525023                                      |
| Branch     | Computer Engineering (CE)                    |
| Institute  | IIIT Bhubaneswar                             |
| Course     | Design and Analysis of Algorithms Laboratory |
| Semester   | B.Tech 3rd Semester                          |
| Instructor | Dr. Ajaya Kumar Dash                         |

---

## About the Repository

- Written entirely in **C**, compiled with **GCC**.
- One top-level folder per lab (`DAA_Lab_01`, `DAA_Lab_02`, …).
- Inside each lab folder, one subfolder per question, named after the
  question itself (e.g. `Q1_Putting_them_in_order`, `Q4_Tower_of_Hanoi`),
  holding that question's `.c` file plus anything it generates.
- File names describe the problem, not just the question number — e.g.
  `Q4_tower_of_hanoi.c` rather than `q4.c`.
- Programs that study growth or performance write their measurements to a
  **CSV** file inside their own question folder, which is then plotted and
  committed as a **PNG** next to it.
- Plots are generated with a small Python + `matplotlib` script
  (`generate_plots.py`) that reads the CSVs — kept in the repo so any plot
  can be regenerated on demand.
- No external C libraries — only the C standard library (`stdio.h`,
  `stdlib.h`, `math.h`, `time.h`).

---

## Repository Structure

```text
DAA_Lab/
│
├── README.md
│
├── DAA_Lab_01/
│   ├── Q1_Putting_them_in_order/
│   │   ├── Q1_order_of_growth.c
│   │   ├── growth_order_data.csv
│   │   ├── Q1_growth_order_plot.png
│   │   └── Q1_growth_order_linechart.png
│   │
│   ├── Q2_Fair_vs_Biased_Coin/
│   │   └── Q2_coin_toss.c
│   │
│   ├── Q3_Performance_Analysis_of_bubble_sort/
│   │   ├── Q3_bubble_sort.c
│   │   ├── bubble_sort_data.csv
│   │   ├── bubble_sort_sorted_data.csv
│   │   ├── Q3_bubble_sort_random_plot.png
│   │   └── Q3_bubble_sort_sorted_plot.png
│   │
│   ├── Q4_Tower_of_Hanoi/
│   │   ├── Q4_tower_of_hanoi.c
│   │   ├── hanoi_data.csv
│   │   ├── Q4_hanoi_plot.png
│   │   └── Q4_hanoi_plot_logscale.png
│   │
│   ├── Q5_Find_the_partition_point/
│   │   └── Q5_partition_point.c
│   │
│   └── Q6_Element_uniqueness/
│       └── Q6_element_uniqueness.c
│
├── DAA_Lab_02/
    │
    ├── Q1_Dictionary_Operations/
    │   ├── q1_dictionary.c
    │   ├── q1_dictionary_benchmark.csv
    │   ├── Q1_dictionary_operations.png
    │   ├── Q1_search.png
    │   ├── Q1_insert.png
    │   ├── Q1_delete.png
    │   ├── Q1_max.png
    │   ├── Q1_min.png
    │   ├── Q1_predecessor.png
    │   └── Q1_successor.png
    │
    ├── Q2_Merge_sort_vs_Modified_merge_sort/
    │   ├── q2_mergesort_compare.c
    │   ├── q2_mergesort_benchmark.csv
    │   ├── Q2_mergesort_comparison.png
    │   └── Q2_mergesort_comparison_loglog.png
    │
    └── Q3_Merging_k_sorted_arrays/
        ├── Q3_kway_merge.c
        ├── kway_merge_data.csv
        ├── Q3_experimentA_vary_k.png
        ├── Q3_experimentA_vary_k_logscale.png
        └── Q3_experimentB_vary_n.png
│
└── DAA_Lab_03/
    │
    ├── Q1_Binary_vs_Ternary_Search/
    │   ├── Q1_binary_vs_ternary_search.c
    │
    ├── Q2_Search_the_Defective_Coin/
    │   ├── Q2_Search_the_Defective_Coin.c
    │
    ├── Q3_Max_and_Min_using_DnC_approach/
    │   ├── Q3_Max_and_Min_using_DnC_approach.c
    │
    ├── Q4_Matrix_multiplication_using_DnC_approach/
    │   ├── Q4_Matrix_multiplication_using_DnC_approach.c
    │
    ├── Q5_Multiply_special_pattern_square_matrix_using_DnC_approach/
    │   ├── Q5_Multiply_special_pattern_square_matrix_using_DnC_approach.c
    │
    ├── Q6_Use_of_loop_invariants_in_sorting/
    │   ├── Q6_Use_of_loop_invariants_in_sorting.c
    └── generate_plots.py
```

---

## Lab Index

| Lab    | Topic                                                    | Date        | Questions | Folder                   |
| ------ | -------------------------------------------------------- | ----------- | --------- | ------------------------ |
| Lab 01 | Growth of functions, empirical analysis, recurrences     | 29 Jul 2026 | 6         | [DAA_Lab_01](DAA_Lab_01) |
| Lab 02 | Dictionary ADT costs, merge sort variants, k-way merging | 05 Aug 2026 | 3         | [DAA_Lab_02](DAA_Lab_02) |
| Lab 03 | Divide and conquer — search, selection, matrix multiplication | 11 Aug 2026 | 6     | [DAA_Lab_03](DAA_Lab_03) |

---

## DAA_Lab_01

> Growth rates, randomised simulation, and counting the work an algorithm
> actually does.

| #   | Question                 | Description                                                                                                                | File                                                                                      |
| --- | ------------------------ | -------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------- |
| 1   | Put them in Order        | Arrange 12 given functions in increasing order of growth for sufficiently large `n`.                                       | [Q1/Q1_order_of_growth.c](DAA_Lab_01/Q1_Putting_them_in_order/Q1_order_of_growth.c)       |
| 2   | Fair vs Biased Coin      | Simulate a coin toss, verify that P(HEAD) → 0.5, then compare a fair coin against biased ones.                             | [Q2/Q2_coin_toss.c](DAA_Lab_01/Q2_Fair_vs_Biased_Coin/Q2_coin_toss.c)                     |
| 3   | Bubble Sort Performance  | Compare a bubble sort that stops early once sorted against one that always runs all `n−1` passes, by counting comparisons. | [Q3/Q3_bubble_sort.c](DAA_Lab_01/Q3_Performance_Analysis_of_bubble_sort/Q3_bubble_sort.c) |
| 4   | Towers of Hanoi          | Simulate the puzzle, tabulate the number of moves for `n` discs, and match it against the closed form.                     | [Q4/Q4_tower_of_hanoi.c](DAA_Lab_01/Q4_Tower_of_Hanoi/Q4_tower_of_hanoi.c)                |
| 5   | Find the Partition Point | Given an array of 0s followed by 1s, locate the exact transition index.                                                    | [Q5/Q5_partition_point.c](DAA_Lab_01/Q5_Find_the_partition_point/Q5_partition_point.c)    |
| 6   | Element Uniqueness       | Check whether `n` given numbers are all distinct, and reason about the cost for large `n`.                                 | [Q6/Q6_element_uniqueness.c](DAA_Lab_01/Q6_Element_uniqueness/Q6_element_uniqueness.c)    |

---

## Highlight — Q1 sidesteps overflow instead of hitting it

`Q1/Q1_order_of_growth.c` handles the trap built into the question paper
directly: three of the twelve functions (`n^(log2 n)`, `3^n`, and a naive
read of the `2^32·n` term) grow fast enough that evaluating them as raw
`double`s either overflows or drowns out every other function on a plot.

The fix is to compare `log2(f(n))` for each function instead of `f(n)`
itself — since `log2` is monotonically increasing, `log2(f(n)) < log2(g(n))`
if and only if `f(n) < g(n)`, so the ordering survives even when the raw
values wouldn't fit in a `double`. Sorting is done with the standard
library's `qsort` over these log2 values.

This also exposes the one deliberate trick in the question: `2^32·n` looks
exponential at a glance, but it is a **constant** (2³² ≈ 4.29 billion) times
`n` — still linear. Evaluated at `n = 10⁶`, it lands at `log2(f(n)) ≈ 51.9`,
comfortably inside the polynomial cluster and nowhere near `3^n` (`≈ 1.58
million`):

```text
Increasing order of growth (n = 10^6):
1/n  <  log2 n  <  n^0.51  <  12*sqrt(n)  <  50*sqrt(n)  <  n*log2(n)
     <  n^2-324  <  100n^2+6n  <  2^32*n  <  2n^3  <  n^(log2 n)  <  3^n
```

---

## DAA_Lab_02

> Dictionary ADT costs across six representations, a Master-theorem
> comparison between 2-way and 3-way merge sort, and two strategies for
> merging k sorted arrays.

| #   | Question                          | Description                                                                                                                                  | File                                                                                                      |
| --- | --------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------- |
| 1   | Dictionary Operations             | Derive and empirically confirm the worst-case cost of Search, Insert, Delete, Max, Min, Predecessor, and Successor across 6 implementations. | [Q1/Q1_dictionary_operations.c](DAA_Lab_02/Q1_Dictionary_Operations/Q1_dictionary_operations.c)           |
| 2   | Merge Sort vs Modified Merge Sort | Compare standard 2-way merge sort against a 3-way variant (split into thirds, 3-way merge), by recurrence and by measured runtime.           | [Q2/Q2_mergesort_comparison.c](DAA_Lab_02/Q2_Merge_sort_vs_Modified_merge_sort/Q2_mergesort_comparison.c) |
| 3   | Merging k Sorted Arrays           | Compare sequential merging (merge in one at a time) against pairwise/tournament merging, for k arrays of n elements each.                    | [Q3/Q3_kway_merge.c](DAA_Lab_02/Q3_Merging_k_sorted_arrays/Q3_kway_merge.c)                               |

---

## Highlight — Q3 makes the O(nk²) vs O(nk log k) gap visible

`Q3_kway_merge.c` runs two experiments: fixing `n` and growing `k`, and
fixing `k` and growing `n`. The first is the interesting one — it is where
sequential merging's quadratic dependence on `k` actually shows up as a
number instead of staying a symbol in a proof.

At `n = 1000` fixed, going from `k = 2` to `k = 256` (a 128× increase in
`k`) makes sequential merging **~4300× slower** (16 µs → 68,788 µs), while
pairwise merging only grows **~840×** (13 µs → 10,887 µs) over the same
range — visibly sub-quadratic, tracking `O(nk log k)` instead of
`O(nk²)`. Both methods produce a correctly sorted array every time (checked
in-program with `is_sorted()`), so the divergence is purely about how the
work is scheduled, not correctness.

---

## DAA_Lab_03

> Divide and conquer, end to end: search, a balance-scale puzzle, a linear
> selection bound, Strassen's matrix multiplication, an O(n²) trick for a
> special matrix pattern, and a loop-invariant proof for selection sort.

| #   | Question                          | Description                                                                                                                        | File                                                                                                                     |
| --- | ---------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------ | -------------------------------------------------------------------------------------------------------------------------- |
| 1   | Binary vs Ternary Search           | Search a sorted array with both strategies, count comparisons, and show binary search wins despite ternary cutting into more pieces. | [Q1/Q1_binary_vs_ternary_search.c](DAA_Lab_03/Q1_Binary_vs_Ternary_Search/Q1_binary_vs_ternary_search.c)                |
| 2   | Search the Defective Coin          | Find the one lighter coin among `n` (or report none) using a balance scale, in ⌊log₂n⌋ + c weighings via divide and conquer.         | [Q2/Q2_defective_coin.c](DAA_Lab_03/Q2_Search_the_Defective_Coin/Q2_defective_coin.c)                                    |
| 3   | Max and Min using D&C              | Find both the max and min of an array using at most `3n/2 − 2` comparisons, instead of the naive `2n − 2`.                            | [Q3/Q3_maxmin_dc.c](DAA_Lab_03/Q3_Max_and_Min_using_DandC/Q3_maxmin_dc.c)                                                |
| 4   | Matrix Multiplication using D&C    | Multiply two `n×n` matrices with Strassen's method — 7 recursive multiplications instead of 8, giving `O(n^log₂7)`.                  | [Q4/Q4_strassen_multiplication.c](DAA_Lab_03/Q4_Matrix_Multiplication_using_DandC/Q4_strassen_multiplication.c)         |
| 5   | Multiply Special-Pattern Matrices  | Exploit a self-similar block pattern `M=[[M1,M2],[M2,M1]]` (recursive down to scalars) to multiply two such matrices in `O(n²)`.      | [Q5/Q5_special_pattern_multiplication.c](DAA_Lab_03/Q5_Multiply_Special_Pattern_Matrices/Q5_special_pattern_multiplication.c) |
| 6   | Loop Invariants in Sorting         | State and prove the loop invariant for selection sort; show best case is no better than worst case.                                  | [Q6/Q6_selection_sort.c](DAA_Lab_03/Q6_Loop_Invariants_in_Sorting/Q6_selection_sort.c)                                   |

---

## Highlight — Q5 turns an O(n³) problem into O(n²) by exploiting structure

The matrices in Q5 aren't arbitrary: `M = [[M1, M2], [M2, M1]]`, and **M1 and
M2 have that same recursive pattern all the way down to single elements.**
That constraint means an `n×n` matrix of this type is fully described by
just `n` numbers, not `n²` — the block structure repeats, so most of the
matrix is redundant.

`Q5_special_pattern_multiplication.c` represents each matrix as a flat array
of `n` "leaf" values instead of an `n×n` grid, and multiplies two of them
using the recurrence `T(n) = 4T(n/2) + O(n)`, which solves to `Θ(n²)` by the
master theorem (the O(n) additions are polynomially smaller than the
`n^log₂4 = n²` term from the four recursive multiplications, so it's case 1).

The measured gap against brute-force `O(n³)` multiplication on the
*expanded* matrices is stark — at `n = 512`, the structured approach takes
**6.4 ms** against **128.6 ms** for brute force, a **~20× speedup that keeps
growing with n.** The structured version also happily continues to
`n = 2048` (**66.9 ms**), a size where a full `O(n³)` brute-force
multiplication was no longer practical to run for comparison.

Both algorithms were cross-checked by expanding the structured matrices to
full `n×n` grids and comparing entrywise — they agree at every size tested.

---

## Results and Artifacts

The three simulation/measurement programs export their data; the plots are
committed next to them inside their own question folder.

| Data                                                                                                                                                                                                               | Plot                                                                                                                                                                                                                                         | Produced by | What it shows                                                                                                                                    |
| ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------- | ------------------------------------------------------------------------------------------------------------------------------------------------ |
| [Q1/growth_order_data.csv](DAA_Lab_01/Q1_Putting_them_in_order/growth_order_data.csv)                                                                                                                              | [Q1/Q1_growth_order_linechart.png](DAA_Lab_01/Q1_Putting_them_in_order/Q1_growth_order_linechart.png)                                                                                                                                        | Q1          | `log2(f(n))` for all 12 functions on a symlog y-axis, so both the tiny (`1/n`) and astronomically large (`3^n`) ends stay readable on one chart. |
| [Q3/bubble_sort_data.csv](DAA_Lab_01/Q3_Performance_Analysis_of_bubble_sort/bubble_sort_data.csv), [Q3/bubble_sort_sorted_data.csv](DAA_Lab_01/Q3_Performance_Analysis_of_bubble_sort/bubble_sort_sorted_data.csv) | [Q3/Q3_bubble_sort_random_plot.png](DAA_Lab_01/Q3_Performance_Analysis_of_bubble_sort/Q3_bubble_sort_random_plot.png), [Q3/Q3_bubble_sort_sorted_plot.png](DAA_Lab_01/Q3_Performance_Analysis_of_bubble_sort/Q3_bubble_sort_sorted_plot.png) | Q3          | Comparisons vs `n` for the early-exit and full-pass bubble sorts, on both random and already-sorted input.                                       |
| [Q4/hanoi_data.csv](DAA_Lab_01/Q4_Tower_of_Hanoi/hanoi_data.csv)                                                                                                                                                   | [Q4/Q4_hanoi_plot.png](DAA_Lab_01/Q4_Tower_of_Hanoi/Q4_hanoi_plot.png), [Q4/Q4_hanoi_plot_logscale.png](DAA_Lab_01/Q4_Tower_of_Hanoi/Q4_hanoi_plot_logscale.png)                                                                             | Q4          | Moves against number of discs, 1 → 20, on both a linear and a log y-axis.                                                                        |

A few conclusions that fall out of the data:

- **Law of large numbers (Q2).** Over 1,000,000 tosses, the observed head
  frequency never drifts far from its programmed bias — the fair coin lands
  at **0.50038** against an expected 0.5, and a `p = 0.7` biased coin lands
  at **0.69989**.
- **Early termination only pays off on nearly-sorted input (Q3).** On random
  data at `n = 2000` the early-exit sort still needs **1,997,824**
  comparisons against **1,999,000** for the full-pass version — barely
  different, since a random array rarely finishes early. On already-sorted
  data at the same `n`, early-exit drops to just **1,999** comparisons
  versus **1,999,000** — a ~1000× difference. Both remain Θ(n²) in the worst
  case; the win is best-case only.
- **The recurrence is exact (Q4).** `T(n) = 2·T(n−1) + 1` solves to `2ⁿ − 1`,
  and the simulated move count matches the closed form for all 20 rows
  tested — 20 discs take exactly **1,048,575** moves.
- **Binary search wins decisively on the partition point (Q5).** On a
  1,000,000-element array, the linear scan needed **333,334** comparisons to
  find a boundary at index 333,333; binary search found the same boundary in
  **20**.

### DAA_Lab_02

| Data                                                                                                              | Plot                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 | Produced by | What it shows                                                                                                                         |
| ----------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | ----------- | ------------------------------------------------------------------------------------------------------------------------------------- |
| [Q1/dictionary_operations_data.csv](DAA_Lab_02/Q1_Dictionary_Operations/dictionary_operations_data.csv)           | [Q1/Q1_dictionary_operations_plot.png](DAA_Lab_02/Q1_Dictionary_Operations/Q1_dictionary_operations_plot.png) (grid), plus one plot per operation — [Search](DAA_Lab_02/Q1_Dictionary_Operations/Q1_search_plot.png), [Insert](DAA_Lab_02/Q1_Dictionary_Operations/Q1_insert_plot.png), [Delete](DAA_Lab_02/Q1_Dictionary_Operations/Q1_delete_plot.png), [Max](DAA_Lab_02/Q1_Dictionary_Operations/Q1_max_plot.png), [Min](DAA_Lab_02/Q1_Dictionary_Operations/Q1_min_plot.png), [Predecessor](DAA_Lab_02/Q1_Dictionary_Operations/Q1_predecessor_plot.png), [Successor](DAA_Lab_02/Q1_Dictionary_Operations/Q1_successor_plot.png) | Q1          | Average time per operation (ns, log scale) vs `n`, for all 6 structures, across all 7 operations.                                     |
| [Q2/mergesort_comparison_data.csv](DAA_Lab_02/Q2_Merge_sort_vs_Modified_merge_sort/mergesort_comparison_data.csv) | [Q2/Q2_mergesort_comparison_plot.png](DAA_Lab_02/Q2_Merge_sort_vs_Modified_merge_sort/Q2_mergesort_comparison_plot.png), [Q2/Q2_mergesort_comparison_loglog_plot.png](DAA_Lab_02/Q2_Merge_sort_vs_Modified_merge_sort/Q2_mergesort_comparison_loglog_plot.png)                                                                                                                                                                                                                                                                                                                                                                       | Q2          | Runtime vs `n` for 2-way and 3-way merge sort, linear and log-log (the log-log plot's matching slopes confirm both are `Θ(n log n)`). |
| [Q3/kway_merge_data.csv](DAA_Lab_02/Q3_Merging_k_sorted_arrays/kway_merge_data.csv)                               | [Q3/Q3_experimentA_vary_k_plot.png](DAA_Lab_02/Q3_Merging_k_sorted_arrays/Q3_experimentA_vary_k_plot.png), [Q3/Q3_experimentA_vary_k_logscale_plot.png](DAA_Lab_02/Q3_Merging_k_sorted_arrays/Q3_experimentA_vary_k_logscale_plot.png), [Q3/Q3_experimentB_vary_n_plot.png](DAA_Lab_02/Q3_Merging_k_sorted_arrays/Q3_experimentB_vary_n_plot.png)                                                                                                                                                                                                                                                                                    | Q3          | Sequential vs pairwise k-way merge time, varying `k` at fixed `n` and varying `n` at fixed `k`.                                       |

A few conclusions that fall out of the Lab 2 data:

- **Data-structure choice trades off which operation is cheap (Q1).** No
  structure wins everywhere: sorted array gives `O(log n)` search but
  `O(n)` insert/delete; doubly linked sorted list gives `O(1)`
  delete/max/min/predecessor/successor but `O(n)` search; unsorted array
  gives `O(1)` insert but `O(n)` everything that depends on order.
- **Both merge sort variants are `Θ(n log n)`, and the benchmark shows it
  (Q2).** At `n = 500,000`, 2-way merge sort averaged **52.2 ms** against
  **57.1 ms** for the 3-way variant over 5 trials — a ~9% constant-factor
  gap, not a different growth class; the log-log plot shows both curves
  running parallel.
- **Sequential k-way merging is quadratic in `k`, and it shows (Q3).** At
  `n = 1000` fixed, growing `k` from 2 to 256 makes sequential merging
  **~4300×** slower (16 µs → 68,788 µs) versus **~840×** for pairwise
  merging (13 µs → 10,887 µs) over the same 128× growth in `k` —
  `O(nk²)` against `O(nk log k)`, made visible rather than just asserted.

### DAA_Lab_03
A few conclusions that fall out of the Lab 3 data:

- **Binary search wins on real numbers, not just asymptotics (Q1).** At
  `n = 1,000,000`, binary search averaged **36.93** comparisons against
  **42.36** for ternary search — ternary's extra per-level comparison costs
  more than the extra split saves, exactly as the `1.26·log₂n` vs `log₂n`
  analysis predicts.
- **The coin weighings track `log₂n` almost exactly (Q2).** At `n = 100,000`
  coins, finding the defective coin (or confirming none exists) took just
  **16** weighings against `log₂(100,000) ≈ 16.6` — the divide-and-conquer
  halving keeps pace with the theoretical bound at every size tested.
- **D&C max-min beats the naive scan by a widening margin (Q3).** At
  `n = 131,072`, the D&C approach needed **196,606** comparisons against
  **262,142** for the naive scan — a fixed ~25% saving that holds at every
  size, matching the `3n/2` vs `2n` bounds exactly for every power of 2.
- **Strassen's crossover is visible, not just theoretical (Q4).** Below
  `n = 256`, brute force is competitive or faster (constant-factor overhead
  from the extra additions/subtractions dominates); by `n = 1024`, Strassen
  takes **663.9 ms** against **1006.5 ms** for brute force — Strassen has
  pulled ahead once the `O(n^2.807)` vs `O(n³)` gap outweighs its overhead.
- **Exploiting matrix structure turns O(n³) into O(n²), and it shows (Q5).**
  At `n = 512`, the structured approach takes **6.4 ms** against **128.6 ms**
  for brute force on the expanded matrices — a ~20× gap that grows with `n`,
  since only `n` values (not `n²`) describe the whole matrix.
- **Selection sort's best case is no better than its worst case (Q6).** A
  fully sorted input at `n = 10,000` still needed **49,995,000**
  comparisons — identical to the random-input case — because the inner loop
  always scans the entire remaining subarray with no early exit.

---

## Complexity Summary

| #   | Program                                                  | Time                            | Space                |
| --- | -------------------------------------------------------- | ------------------------------- | -------------------- |
| 1   | Growth ordering (`qsort` over 12 functions' log2 values) | Θ(F log F) for F = 12 functions | Θ(F)                 |
| 2   | Coin toss simulation                                     | Θ(n) in the number of tosses    | Θ(1)                 |
| 3   | Bubble sort — early termination                          | Ω(n) best, O(n²) worst          | Θ(1) auxiliary       |
| 3   | Bubble sort — full passes                                | Θ(n²) always                    | Θ(1) auxiliary       |
| 4   | Towers of Hanoi                                          | Θ(2ⁿ) moves                     | Θ(n) recursion depth |
| 5   | Partition point — linear scan                            | Θ(n)                            | Θ(1)                 |
| 5   | Partition point — binary search                          | Θ(log n)                        | Θ(1)                 |
| 6   | Element uniqueness — brute force                         | O(n²)                           | Θ(1) auxiliary       |
| 6   | Element uniqueness — sort and scan                       | Θ(n log n)                      | Θ(n) auxiliary       |
| 6   | Element uniqueness — hashing                             | Θ(n) average                    | Θ(n) auxiliary       |

**DAA_Lab_02**

| #   | Program                                        | Time                                                        | Space           |
| --- | ---------------------------------------------- | ----------------------------------------------------------- | --------------- |
| 1   | Dictionary — unsorted array                    | Search/Max/Min/Pred/Succ Θ(n), Insert Θ(1), Delete Θ(1)     | Θ(n)            |
| 1   | Dictionary — sorted array                      | Search Θ(log n); Insert/Delete Θ(n); Max/Min/Pred/Succ Θ(1) | Θ(n)            |
| 1   | Dictionary — singly linked, unsorted           | Search/Max/Min/Pred/Succ Θ(n), Insert Θ(1), Delete Θ(n)     | Θ(n)            |
| 1   | Dictionary — singly linked, sorted             | Search/Insert/Delete/Max Θ(n); Min/Pred Θ(1)/Θ(n)\*         | Θ(n)            |
| 1   | Dictionary — doubly linked, unsorted           | Search/Max/Min/Pred/Succ Θ(n), Insert Θ(1), Delete Θ(1)     | Θ(n)            |
| 1   | Dictionary — doubly linked, sorted             | Search/Insert Θ(n); Delete/Max/Min/Pred/Succ Θ(1)           | Θ(n)            |
| 2   | Merge sort (2-way)                             | Θ(n log₂ n)                                                 | Θ(n) auxiliary  |
| 2   | Merge sort (3-way, modified)                   | Θ(n log₃ n) = Θ(n log n)                                    | Θ(n) auxiliary  |
| 3   | k-way merge — sequential (Method 1)            | O(nk²)                                                      | Θ(nk) auxiliary |
| 3   | k-way merge — pairwise / tournament (Method 2) | Θ(nk log k)                                                 | Θ(nk) auxiliary |

\* Predecessor is Θ(n) in general (must traverse to find it), but Θ(1) at
the point a traversal has already reached the target — see `Q1`'s inline
notes for the exact accounting used.

**DAA_Lab_03**

| #   | Program                                          | Time                            | Space                       |
| --- | ------------------------------------------------- | -------------------------------- | ----------------------------- |
| 1   | Binary search                                    | Θ(log n)                        | Θ(1)                        |
| 1   | Ternary search                                   | Θ(log₃ n), ~1.26× more comparisons than binary | Θ(1)      |
| 2   | Defective coin — divide and conquer              | Θ(log n) weighings              | Θ(log n) recursion depth    |
| 3   | Max-Min — divide and conquer                     | Θ(n), ≤ 3n/2 − 2 comparisons     | Θ(log n) recursion depth    |
| 3   | Max-Min — naive scan                             | Θ(n), 2n − 2 comparisons         | Θ(1)                        |
| 4   | Strassen's matrix multiplication                 | O(n^log₂7) ≈ O(n^2.807)         | Θ(n²) auxiliary             |
| 4   | Brute-force matrix multiplication                | Θ(n³)                           | Θ(1) auxiliary              |
| 5   | Special-pattern matrix multiplication            | Θ(n²) (compact n-value representation) | Θ(n) per matrix, Θ(n²) to expand |
| 6   | Selection sort                                   | Θ(n²) worst **and** best case   | Θ(1) auxiliary              |

---

## Topics Covered

**Analysis**

- [x] Asymptotic notation — Θ, O, Ω
- [x] Ordering functions by rate of growth
- [x] Polynomial vs superpolynomial vs exponential growth
- [x] Constant factors and the "big constant looks scary" trap
- [x] Best case vs worst case, and why early exits don't change a bound
- [x] Counting primitive operations as a machine-independent cost model

**Algorithms**

- [x] Sorting (`qsort`) applied to growth-rate comparison
- [x] Bubble sort and its early-termination variant
- [x] Binary search / partition point
- [x] Brute-force pairwise comparison
- [x] Sort-then-scan and hash-table approaches to duplicate detection
- [x] Recursion (Towers of Hanoi)

**Recurrences and Randomisation**

- [x] Solving `T(n) = 2T(n−1) + 1`
- [x] Verifying a closed form against simulation
- [x] Monte Carlo simulation and the law of large numbers
- [x] Biased sampling
- [x] Master theorem (`T(n) = aT(n/b) + f(n)`, case 2, applied to 2-way and 3-way merge sort)
- [ ] Amortised analysis

**Data Structures (Lab 02)**

- [x] Dictionary ADT (Search, Insert, Delete, Max, Min, Predecessor, Successor)
- [x] Arrays — sorted and unsorted
- [x] Singly linked lists — sorted and unsorted
- [x] Doubly linked lists — sorted and unsorted
- [x] Cost trade-offs between six representations of the same interface

**Divide and Conquer (Lab 02)**

- [x] Merge sort and its 2-way merge subroutine
- [x] Generalising merge sort to a k-way split (3-way merge sort)
- [x] Merging k sorted arrays — sequential vs pairwise/tournament strategy
- [x] Recognising the same doubling structure behind merge sort and pairwise k-way merging

**Divide and Conquer (Lab 03)**

- [x] Binary vs ternary search — comparing split factor against per-level comparison cost
- [x] The counterfeit/defective-coin problem — decision-based divide and conquer with a balance scale
- [x] Simultaneous maximum and minimum — halving the comparison count via paired recursion
- [x] Strassen's matrix multiplication — trading 8 recursive multiplications for 7
- [x] Exploiting problem structure to beat the generic bound (special-pattern matrices, O(n³) → O(n²))
- [x] Loop invariants — initialization, maintenance, termination as a correctness proof technique
- [x] Master theorem applied to T(n)=4T(n/2)+O(n) (Q5) and T(n)=7T(n/2)+O(n²) (Q4, Strassen)

---

## Technologies Used

| Tool                         | Purpose                                   |
| ---------------------------- | ----------------------------------------- |
| C (C99 / C11)                | Implementation language                   |
| GCC                          | Compilation                               |
| C standard library           | `stdio.h`, `stdlib.h`, `math.h`, `time.h` |
| Python + matplotlib + pandas | Generating PNG plots from CSV data        |
| CSV                          | Exporting measurements for plotting       |
| VS Code + WSL                | Editor and development environment        |
| Git and GitHub               | Version control                           |

---

## Compilation and Execution

Each question lives in its own folder, so `cd` into it before compiling:

```bash
cd DAA_Lab_01/Q1_Putting_them_in_order
gcc -O2 -o Q1 Q1_order_of_growth.c -lm && ./Q1   # needs -lm (log2/pow/sqrt)

cd ../Q2_Fair_vs_Biased_Coin
gcc -o Q2 Q2_coin_toss.c && ./Q2

cd ../Q3_Performance_Analysis_of_bubble_sort
gcc -o Q3 Q3_bubble_sort.c && ./Q3

cd ../Q4_Tower_of_Hanoi
gcc -o Q4 Q4_tower_of_hanoi.c && ./Q4

cd ../Q5_Find_the_partition_point
gcc -o Q5 Q5_partition_point.c && ./Q5

cd ../Q6_Element_uniqueness
gcc -O2 -o Q6 Q6_element_uniqueness.c && ./Q6
```

Recommended flags while working:

```bash
gcc -std=c11 -Wall -Wextra -O2 file.c -o out -lm
```

**On Windows** (MinGW-w64), replace `-o Q1` with `-o Q1.exe` and run `Q1.exe`.

**Note on generated files.** Q1, Q3, and Q4 write their CSVs into the
_current working directory_ under a fixed name (`growth_order_data.csv`,
`bubble_sort_data.csv`, `bubble_sort_sorted_data.csv`, `hanoi_data.csv`).
Running them from inside their own question folder (as above) will overwrite the
committed copies there. Run from a scratch directory instead if you want to
keep those files untouched:

```bash
mkdir -p /tmp/daa && cd /tmp/daa
gcc "$OLDPWD/DAA_Lab_01/Q4_Tower_of_Hanoi/Q4_tower_of_hanoi.c" -o q4 && ./q4
```

To regenerate the plots after re-running the programs, run from
`DAA_Lab_01/` so the script can see every question folder:

```bash
cd DAA_Lab_01
pip install pandas matplotlib --break-system-packages
python3 generate_plots.py
```

### DAA_Lab_02

```bash
cd DAA_Lab_02/Q1_Dictionary_Operations
gcc -O2 -o Q1 Q1_dictionary_operations.c && ./Q1

cd ../Q2_Merge_sort_vs_Modified_merge_sort
gcc -O2 -o Q2 Q2_mergesort_comparison.c && ./Q2

cd ../Q3_Merging_k_sorted_arrays
gcc -O2 -o Q3 Q3_kway_merge.c && ./Q3
```

### DAA_Lab_03

```bash
cd DAA_Lab_03/Q1_Binary_vs_Ternary_Search
gcc -O2 -o Q1 Q1_binary_vs_ternary_search.c && ./Q1

cd ../Q2_Search_the_Defective_Coin
gcc -O2 -o Q2 Q2_defective_coin.c -lm && ./Q2

cd ../Q3_Max_and_Min_using_DandC
gcc -O2 -o Q3 Q3_maxmin_dc.c && ./Q3

cd ../Q4_Matrix_Multiplication_using_DandC
gcc -O2 -o Q4 Q4_strassen_multiplication.c && ./Q4

cd ../Q5_Multiply_Special_Pattern_Matrices
gcc -O2 -o Q5 Q5_special_pattern_multiplication.c && ./Q5

cd ../Q6_Loop_Invariants_in_Sorting
gcc -O2 -o Q6 Q6_selection_sort.c && ./Q6
```

Regenerate the plots after re-running the programs from `DAA_Lab_03/`:

```bash
cd DAA_Lab_03
pip install pandas matplotlib --break-system-packages
python3 generate_plots.py
```

---

## Repository Conventions

- One top-level folder per lab, named `DAA_Lab_<nn>`, containing one subfolder per question.
- One subfolder per question, named `Q<number>_<question_title>` (e.g.
  `Q3_Performance_Analysis_of_bubble_sort`), holding that question's `.c`
  file plus anything it generates (CSV data, PNG plots).
- Sources are named `Q<number>_<short_description>.c`.
- Generated CSVs keep the name of the analysis they describe, and their
  plots share that name with a `.png` extension.

---

## License

Coursework, published for reference and learning. Feel free to read, run and
learn from it; please do not submit it as your own.

---

<p align="center">
  Maintained by <strong>Bighnesh</strong> · CE, IIIT Bhubaneswar
</p>
