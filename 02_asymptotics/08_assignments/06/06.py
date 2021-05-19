from numpy.random import randint, choice
from numpy import sqrt, log2
import numpy as np
import os
from tqdm import tqdm
from matplotlib import pyplot as plt
from matplotlib import rc
rc('font', **{'family': 'serif', 'serif': ['Computer Modern Roman']})
rc('text', usetex=True)
rc('text.latex', preamble=r'\usepackage[utf8]{inputenc}')
rc('text.latex', preamble=r'\usepackage[russian]{babel}')

from functions import measure_time, normalize_by_last


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


def generate_random_int(C):
    return randint(C)


def is_prime(a):
    return all(a % i for i in range(2, a))


def generate_random_prime_int(prime_numbers):
    return choice(prime_numbers)


MAX = 10000
C_list = list(range(10, 1000, 20))
t_list_medium, t_list_worst = [], []
for C in tqdm(C_list):
    t_sum_medium, t_sum_worst = 0.0, 0.0
    prime_numbers = [i for i in range(C) if is_prime(i)]
    for i in range(MAX):
        x_medium = generate_random_int(C)
        _, t_medium = measure_time(primes, [x_medium])
        x_worst = generate_random_prime_int(prime_numbers)
        _, t_worst = measure_time(primes, [x_worst])
        t_sum_medium += t_medium
        t_sum_worst += t_worst
    t_sum_medium /= MAX
    t_sum_worst /= MAX
    t_list_medium.append(t_sum_medium)
    t_list_worst.append(t_sum_worst)

# 1. sqrt(C) log(C)
t_list_1 = [sqrt(C) + log2(C) for C in C_list]

# 2. sqrt(C) log(C)
t_list_2 = [sqrt(C) * log2(C) for C in C_list]

# 3. log(C)
t_list_3 = [log2(C) for C in C_list]

# 4. sqrt(C)
t_list_4 = [sqrt(C) for C in C_list]

# 5. C
t_list_5 = [C for C in C_list]

val_to_normalize = np.max([t_list_medium, t_list_worst, t_list_1, t_list_2, t_list_3, t_list_4, t_list_5])
t_list_medium = [e * 9*10**3 for e in t_list_medium]
t_list_worst = [e * 3.7*10**3 for e in t_list_worst]
t_list_1 = normalize_by_last(t_list_1, val_to_normalize)
t_list_2 = normalize_by_last(t_list_2, val_to_normalize)
t_list_3 = normalize_by_last(t_list_3, val_to_normalize)
t_list_4 = normalize_by_last(t_list_4, val_to_normalize)
t_list_5 = normalize_by_last(t_list_5, val_to_normalize)

plt.figure()
plt.plot(C_list, t_list_medium, color='gray', markersize=5, marker='o', linewidth=2, linestyle='solid', label='general case')
plt.plot(C_list, t_list_worst, color='black', markersize=5, marker='o', linewidth=2, linestyle='solid', label='worst case')
plt.plot(C_list, t_list_1, linewidth=2, linestyle='solid', label='$\sqrt{C} + \log(C)$')
plt.plot(C_list, t_list_2, linewidth=2, linestyle='solid', label='$\sqrt{C} \log(C)$')
plt.plot(C_list, t_list_3, linewidth=2, linestyle='solid', label='$\log(C)$')
plt.plot(C_list, t_list_4, linewidth=2, linestyle='solid', label='$\sqrt{C}$')
plt.plot(C_list, t_list_5, linewidth=2, linestyle='solid', label='$C$')
plt.grid(linewidth=1, linestyle='dotted', color='gray', alpha=0.5)
plt.ylim([0, 0.05])
plt.xlabel('C', fontsize=14, fontweight='bold')
plt.ylabel('t, a.u.', fontsize=14, fontweight='bold')
plt.legend()
path_to_save = '/'.join(os.path.abspath(__file__).split('.')[:-1]) + '.png'
plt.savefig(path_to_save, bbox_inches='tight', dpi=500)
plt.close()
