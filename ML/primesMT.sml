 
(* Modified From source: https://stackoverflow.com/questions/48247694/creating-a-list-of-prime-numbers-up-to-a-given-n-value-in-sml *)

fun create_list (current_num, size) = if current_num > size then [] else current_num :: create_list (current_num + 1, size)

fun sqrtMax max = floor(Real.Math.sqrt(real(max)))

fun sieve ([], max) = []
    | sieve (n::ns, max) = 
    if n < max
        then n::sieve(List.filter (fn m => m mod n > 0) ns, max)
    else
        n::ns

(*
fun partition(start, num, partitionTimes) = 
    let
        val newNum = num div partitionTimes
    in
        if partitionTimes = 1
        then sieve( create_list(start, num), sqrtMax(num)) 
        else
        sieve(create_list(start, newNum), sqrtMax(newNum)) @ partition( newNum+1, num, partitionTimes - 1)
    end;
*)

(*
fun startPartition n = 
    let
        val partitions = n mod 1000000
    in
        if partitions > 0
        then partition(2, n, partitions)
        else
            partition(2, n, 1)
    end;
*)

fun primes n = sieve(create_list(2, n), sqrtMax(n)); 
(*fun primes n = startPartition n *)

fun printList xs = print((String.concatWith "\n" (map Int.toString xs)) ^ "\n");

val myPrimes = primes( 1000000 );
printList( myPrimes );
