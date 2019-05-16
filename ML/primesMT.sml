
fun test_bit ( bit_block, n ) =
    let
        val i = n div 32
        val pos = n mod 32
        val new_bit = BitArray.array(1, true)
        val flag = BitArray.lshift(new_bit, pos)
    in
        if BitArray.eqBits( bit_block, flag) 
            then true
        else false
    end;

fun set_primes( until ) = 
    let 
        val bit_block = BitArray.array( until )
        fun check_Set( bit_block, n ) = 
            if test_bit( bit_block, n ) 
            then true
            else false
