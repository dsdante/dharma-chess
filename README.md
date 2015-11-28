## Dharma Chess

A simple chess program for my C programming practicing

Usage: dchess [OPTION...]  
  -h, --help               display this help and exit  
  -l, --log-level=LEVEL    console logging verbosity, from -1 (none) to 7 (debug)  
  -t, --test               run tests

To do:

  1. Console PvP; indicating illegal moves, checkmates, etc --- Debugging
  2. Implement the [CECP](https://en.wikipedia.org/wiki/Chess_Engine_Communication_Protocol) and [UCI](https://en.wikipedia.org/wiki/Universal_Chess_Interface) interfaces
  3. A simple minimax AI
  4. GUI (as a separate executable)
  5. Network multiplayer
  6. A more advanced AI (tree pruning, parallelization, etc)

The common chess terminology (just in case):

  * file: vertical line of squares
  * rank: horizontal line of squares
  * en passant: [the en passant capture](https://en.wikipedia.org/wiki/En_passant)
