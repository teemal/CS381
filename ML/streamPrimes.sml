
(* Start by defining a stream. That consists of a pair head and tail of a stream *)
datatype 'a stream = Null |  Cons of 'a * (unit -> 'a stream);

exception Empty;

fun tl(s) = 
    case s of
        Null => Null
        |Cons(_, t) => t();

fun hd(s) = 
    case s of
        Null => raise Empty
        |Cons(h, _) => h;

fun takeN( s, n) = 
    case (s, n) of
        (_, 0) => []
        | (Cons(h, t), n) => h :: (takeN (t(), n-1));

fun filter (f: 'a -> bool) (s: 'a stream): 'a stream = 
    case s of 
        Cons(h, t) => if f(h) then Cons(h, fn () => filter f (t())) else filter f (t());

fun sift p s = 
    filter (fn n => n mod p <> 0) s

fun sieve s = 
    case s of 
        Null => Null
        | Cons(s, t) => Cons(s, fn () => sieve( sift s (t())));

fun nats n = Cons(n , fn()=> nats(n+1))
val primes = sieve(nats 2);
takeN(primes, 312);
