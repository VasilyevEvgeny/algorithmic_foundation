def primes(x):
    result = []
    i = 2
    while i * i <= x:
        while x % i == 0:
            result.append(i)
            x //= i
        i += 1
    if x != 1:
        result.append(x)
    return result