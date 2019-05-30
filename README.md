# CS381
## This repo contains implementations of a Sieve of Eratosthenes, an algorithm to find prime numbers up to N. It does this by creating a list, starting at 2, and runs through each integer and removes all composite digits. There is one implementation in C, one in ML, and one in Elixir.

### To run:
1. Clone repo

### C:
1. `cd to/C/directory`
2. run `make`
3. run `./primesMT` + `-t num_threads -u upper_bound` where num_threads is the number of threads you want to dedicate and upper_bound is the highest possible prime to find
4. Or to run automated tests this will execute and time for various upper bounds and threads. Use `./testPrimesC`

### Elixir
1. `cd to/Elixir/directory`
2. run `elixir primesMT.ex` and follow prompt

### ML
1. Enter ML directory
2. run the bash script`./testSML` to run the automated tests.
3. or `./` any of the programs to test individually ex `./500KPrimes`
4. Otherwise edit line number 46 `val myPrimes = primes( "Place Upper Bound Here" );`
5. After changing primesMT.sml can be compiled with mlton using `mlton primesMT.sml`
6. Then execute. 


##### Created by Taylor Mallory and Mike Winberry


