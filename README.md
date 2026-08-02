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
└── DAA_Lab_01/
    ├── Q1_Putting_them_in_order/
    │   ├── Q1_order_of_growth.c
    │   ├── growth_order_data.csv
    │   ├── Q1_growth_order_plot.png
    │   └── Q1_growth_order_linechart.png
    │
    ├── Q2_Fair_vs_Biased_Coin/
    │   └── Q2_coin_toss.c
    │
    ├── Q3_Performance_Analysis_of_bubble_sort/
    │   ├── Q3_bubble_sort.c
    │   ├── bubble_sort_data.csv
    │   ├── bubble_sort_sorted_data.csv
    │   ├── Q3_bubble_sort_random_plot.png
    │   └── Q3_bubble_sort_sorted_plot.png
    │
    ├── Q4_Tower_of_Hanoi/
    │   ├── Q4_tower_of_hanoi.c
    │   ├── hanoi_data.csv
    │   ├── Q4_hanoi_plot.png
    │   └── Q4_hanoi_plot_logscale.png
    │
    ├── Q5_Find_the_partition_point/
    │   └── Q5_partition_point.c
    │
    └── Q6_Element_uniqueness/
        └── Q6_element_uniqueness.c
```

---

## Lab Index

| Lab    | Topic                                                | Date        | Questions | Folder                   |
| ------ | ---------------------------------------------------- | ----------- | --------- | ------------------------ |
| Lab 01 | Growth of functions, empirical analysis, recurrences | 29 Jul 2026 | 6         | [DAA_Lab_01](DAA_Lab_01) |

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
- [ ] Master theorem
- [ ] Amortised analysis

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
