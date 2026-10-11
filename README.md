# Rashbot

Rashbot is a chess engine written from scratch in C++. It includes a UCI-compatible engine for chess interfaces and Lichess, plus an SDL3 graphical interface for playing locally.

The project is named after Rashid Nezhmetdinov. Its long-term goal is to combine solid search with an aggressive, tactical playing style inspired by his games.

## Features

### Board and move generation

- Bitboard representation with separate bitboards for all 12 piece types
- State stack for efficient make-and-undo operations
- Pseudo-legal move generation followed by king-safety validation
- Castling, en passant, captures, and all four promotion choices
- Check, checkmate, and stalemate detection
- FEN position loading
- Fifty-move, threefold-repetition, and insufficient-material draw detection
- Fixed-seed Zobrist hashing for position identification and repetition detection
- Perft and perft-divide testing

### Search and evaluation

- Negamax search with alpha-beta pruning
- Iterative deepening
- Quiescence search for tactically unstable leaf positions
- Move ordering for improved alpha-beta efficiency
- Material evaluation with piece-square tables
- Mate-distance scoring
- Node counting
- Search deadlines and externally requested cancellation

### Interfaces

- UCI command-line executable: `rashbot`
- SDL3 graphical executable: `rashbot-gui`
- Background UCI search so `stop` and `quit` can interrupt a search
- Time management using UCI clock and increment information
- Compatibility with `lichess-bot`

## Perft results

Move generation has been checked against the standard starting-position perft values:

| Depth | Nodes |
| ---: | ---: |
| 1 | 20 |
| 2 | 400 |
| 3 | 8,902 |
| 4 | 197,281 |
| 5 | 4,865,609 |
| 6 | 119,060,324 |

## Requirements

- A C++17-compatible compiler
- CMake 3.20 or newer
- SDL3
- SDL3_image
- A platform with standard C++ thread support

On macOS, the dependencies can be installed with Homebrew:

```sh
brew install cmake sdl3 sdl3_image
```

## Building

Configure the project:

```sh
cmake -S . -B build
```

Build both executables:

```sh
cmake --build build
```

Alternatively, build one target:

```sh
cmake --build build --target rashbot
cmake --build build --target rashbot-gui
```

The resulting executables are:

```text
build/rashbot
build/rashbot-gui
```

## Running the GUI

```sh
./build/rashbot-gui
```

Controls:

- Click a piece and then a destination square to move.
- Press `Space` to flip the board.
- Press `Backspace` to undo a move.
- Press `Escape` to quit.

The engine colour and search depth used by the GUI are currently configured in `main.cpp`.

## Running as a UCI engine

Start Rashbot from a terminal:

```sh
./build/rashbot
```

A basic manual test is:

```text
uci
isready
ucinewgame
position startpos moves e2e4 e7e5 g1f3
go depth 5
quit
```

Rashbot responds to a completed search with a move such as:

```text
bestmove b8c6
```

The UCI interface supports the core commands needed for play, including:

- `uci`
- `isready`
- `ucinewgame`
- `position startpos ...`
- `position fen ...`
- `go depth ...`
- `go movetime ...`
- Clock-based `go` commands using `wtime`, `btime`, `winc`, and `binc`
- `go infinite`
- `stop`
- `quit`

Pondering and configurable `setoption` parameters are not currently supported.

## Playing on Lichess

You can play against rashbot [here](https://lichess.org/@/rashbot), although right now this will only work when my computer is on. In the future I plan on running it 24/7 on either a Raspberry Pi or Cloud VPS.

## Project structure

| File | Purpose |
| --- | --- |
| `main.cpp` | SDL GUI and local human-versus-engine game loop |
| `uci_main.cpp` | Entry point for the UCI executable |
| `uci.cpp` / `uci.h` | UCI command handling, time controls, and search-thread management |
| `chessboard.cpp` / `chessboard.h` | Position state, move generation, legality, FEN loading, hashing, and draw detection |
| `search.cpp` / `search.h` | Evaluation, move ordering, iterative deepening, alpha-beta search, and quiescence |
| `perft.cpp` / `perft.h` | Perft and perft-divide tests |
| `helper.h` | Bitboard constants and helper functions |
| `assets/` | Piece textures used by the SDL GUI |

## Planned improvements

- Transposition table using the existing Zobrist hashes
- A custom opening book based primarily on games by Rashid Nezhmetdinov and other attacking players
- More sophisticated positional and attacking evaluation terms
- Principal-variation reporting and additional UCI information output
- Multi-thread search
- Search performance benchmarking and optimization
- Optional pondering
- Non-blocking search in the SDL GUI
- Headless Linux deployment for continuous Lichess availability
