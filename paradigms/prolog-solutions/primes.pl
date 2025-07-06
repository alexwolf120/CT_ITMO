prime(2).
prime(3).
prime(5).
prime(N) :- 
    N mod 2 > 0,
    \+ primet(N, 3).

primet(N, R) :- 
    0 is mod(N, R).
primet(N, R) :- 
    (R * R) < N,
    R2 is R + 2,
    primet(N, R2).

composite(N) :- \+ prime(N).

prime_divisors(N, Divisors) :-
    prime_divisors(N, 2, Divisors),
    !.

prime_divisors(1, _, []).

prime_divisors(N, P, [P | Divisors]) :-
    0 is mod(N,P),
    N1 is N // P,
    prime_divisors(N1, P, Divisors).
prime_divisors(N, P, Divisors) :-
    P1 is P + 1,
    prime_divisors(N, P1, Divisors).
cube_divisors(N, Divisors):-
    N1 is (N * N * N),
    prime_divisors(N1, Divisors).