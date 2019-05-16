 
fun enum (n, m) = if n > m then [] else n :: enum (n + 1, m)
fun sieve [] = []
    | sieve (n::ns) = n :: sieve (List.filter (fn m => m mod n > 0) ns)
fun primes n = sieve( enum( 2, n) );

fun printList xs = print((String.concatWith "\n" (map Int.toString xs)) ^ "\n");

val myPrimes = primes( 1000000 );
printList( myPrimes );
