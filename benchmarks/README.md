# Rashbot Benchmarks

This directory tracks Rashbot's playing-strength and performance tests.

## Testing methodology

- Matches are run using Fastchess.
- Games use paired openings with colours reversed.
- Comparisons between Rashbot versions use identical settings.
- Elo estimates from small samples should be treated as approximate.

## Results

| Version | Opponent | Time control | Games | W-D-L | Score | Elo difference | Estimated elo |
|---|---|---:|---:|---:|---:|---:|---:|
| benchmark-v1 | Stockfish 1800 | 30+0.3 | 50 | 16-6-28 | 38.0% | -85 ±104 | 1715 ±104

## Version notes

### benchmark-v1

Initial benchmarked version containing:

- Bitboard move generation
- Alpha-beta negamax
- Iterative deepening
- Quiescence search
- Move ordering
- Material and piece-square evaluation
- UCI time management