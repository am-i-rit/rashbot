# Rashbot

Rashbot is a chess engine written from scratch in C++17. It includes a bitboard-based chess implementation, an iterative-deepening Negamax search, an SDL3 graphical interface, perft testing, and a basic Universal Chess Interface (UCI) executable.

## Features

### Board representation and rules

- Separate 64-bit bitboards for all 12 piece types
- Precomputed attack tables and sliding-piece rays
- State stack for fast move execution and undo
- Pseudo-legal move generation for every piece
- Legal-move validation through king-safety checks
- Castling on both sides
- En passant captures
- Queen, rook, bishop, and knight promotions
- Checkmate and stalemate detection

### Search and evaluation

- Iterative-deepening Negamax search
- Alpha-beta pruning
- Quiescence search for captures, promotions, and check evasions
- Material and piece-square-table evaluation
- Capture and promotion move ordering
- Previous-iteration best-move ordering
- Node counting and search information output

### Interfaces

- SDL3 graphical interface
- Mouse-controlled move selection
- Promotion selection
- Board flipping
- Move undo
- Human-versus-engine gameplay
- Basic UCI executable for use from a terminal or compatible chess GUI
- Separate GUI and UCI build targets

## Perft results

The move generator has been checked against the standard starting-position perft values:

| Depth | Nodes |
|------:|------:|
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

## Building on macOS

Install the dependencies with Homebrew:

```sh
brew install cmake sdl3 sdl3_image
```

Clone and build the project:

```sh
git clone https://github.com/am-i-rit/rashbot.git
cd rashbot

cmake -S . -B build
cmake --build build
```

Build only one executable if desired:

```sh
cmake --build build --target chess-engine
cmake --build build --target chess-engine-uci
```

Run the graphical interface:

```sh
./build/chess-engine
```

Run the UCI executable:

```sh
./build/chess-engine-uci
```

## Building on Windows

### 1. Install the development tools

Install the following:

- [Visual Studio](https://visualstudio.microsoft.com/downloads/) with the **Desktop development with C++** workload
- Git
- CMake
- [vcpkg](https://learn.microsoft.com/vcpkg/get_started/get-started)

The Visual Studio C++ workload includes the MSVC compiler and CMake support.

### 2. Install vcpkg

Open PowerShell and run:

```powershell
git clone https://github.com/microsoft/vcpkg.git C:\dev\vcpkg
cd C:\dev\vcpkg
.\bootstrap-vcpkg.bat
```

Install SDL3 and SDL3_image with PNG support:

```powershell
.\vcpkg install sdl3:x64-windows "sdl3-image[png]:x64-windows"
```

### 3. Clone and configure Rash-bot

```powershell
cd C:\dev
git clone https://github.com/am-i-rit/rashbot.git
cd rashbot
```

Configure the project using the vcpkg CMake toolchain:

```powershell
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake
```

If vcpkg was installed somewhere else, replace `C:/dev/vcpkg` with its actual location.

### 4. Build the executables

```powershell
cmake --build build --config Release
```

The resulting executables should be located at:

```text
build\Release\chess-engine.exe
build\Release\chess-engine-uci.exe
```

### 5. Run the programs

Run the graphical interface:

```powershell
.\build\Release\chess-engine.exe
```

Run the UCI executable:

```powershell
.\build\Release\chess-engine-uci.exe
```

To use Rash-bot in a chess GUI, select `chess-engine-uci.exe` as the engine executable.

## GUI controls

- Click a piece and then its destination square to move.
- Select a displayed piece when promoting a pawn.
- Press `Space` to flip the board.
- Press `Backspace` to undo a move.
- Press `Escape` to quit.

The human currently plays White and the engine plays Black. The engine side and search depth can be changed through `engineTurn` and `engineDepth` in `main.cpp`.

## UCI usage

The committed UCI implementation supports:

- `uci`
- `isready`
- `ucinewgame`
- `position startpos`
- `position startpos moves ...`
- `go depth <depth>`
- `quit`

Example session:

```text
uci
isready
ucinewgame
position startpos moves e2e4 e7e5
go depth 5
quit
```

The engine reports search information and returns a move in this format:

```text
info depth 5 nodes 123456
bestmove g1f3
```

## Project structure

| File | Purpose |
|---|---|
| `main.cpp` | SDL3 interface and human-versus-engine game loop |
| `uci_main.cpp` | Entry point for the UCI executable |
| `uci.cpp` / `uci.h` | UCI parsing and move conversion |
| `chessboard.cpp` / `chessboard.h` | Board state, move generation, move execution, and legality |
| `search.cpp` / `search.h` | Evaluation, move ordering, and Negamax search |
| `perft.cpp` / `perft.h` | Perft and perft-divide functions |
| `helper.h` | Bitboard constants and helper functions |
| `assets/pieces/` | Piece textures used by the SDL3 interface |

## Current limitations

- UCI searches are fixed-depth and run synchronously.
- The UCI `stop` command is not yet supported.
- UCI `position fen` is not yet implemented.
- Draw tracking is incomplete because the half-move clock and Zobrist hash are not updated yet.
- Draw detection is not currently enabled during search.
- Evaluation uses material and middlegame piece-square tables without game-phase interpolation.
- The engine does not currently use a transposition table.

## Planned improvements

- Clock-controlled UCI search
- Interruptible search and UCI `stop`
- FEN position loading
- Reliable fifty-move and repetition detection
- Zobrist hashing and a transposition table
- Separate middlegame and endgame evaluation
- Additional move-ordering and search heuristics
