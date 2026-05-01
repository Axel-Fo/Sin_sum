# Signal Approximation & Visualization

C program that reads signal parameters, displays a grid of theoretical vs approximated signals, and finds the maximum using dichotomic search.

**Author:** Axel Fouet (375085)

---

## Build & run

```bash
make        # compile
./projet    # run
```

---

## Features

- **Input validation** — reads and checks parameters via `saisie_val` and `detec_erreur`
- **Signal display** — prints a grid comparing theoretical (`_thr`) and approximated (`_sum`) signals
- **Max search** — recursive dichotomic search (`max_dicho`) to find the signal maximum over one period

## Supported signals

- `TRIANGLE` — no discontinuity, max converges to 1
- `SAWTOOTH` and `SQUARE` — discontinuities near the maximum cause the Gibbs phenomenon, max converges to ≈ 1.18

## Complexity

| Task | Complexity |
|---|---|
| Fill signal grid | O(nbL × nbN) |
| Display grid | O(nbL²) |
| Overall task 2 | O(nbL² + nbL × nbN) |
