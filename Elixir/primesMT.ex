defmodule Sieve do
  def primes_to(limit), do: sieve(Enum.to_list(2..limit))

  def sieve([h|t]), do: [h|sieve(t -- for n <- 1..div(length(t),2), do: h*n)]
  def sieve([]), do: []
  def call(upper_bound), do: IO.puts(["list is \n", Enum.join(Sieve.primes_to(upper_bound), " \n")])
end

upper_bound = IO.gets("Enter upper bound: \n") |> String.trim_trailing
upper_bound = String.to_integer(upper_bound)
Sieve.call(upper_bound)

