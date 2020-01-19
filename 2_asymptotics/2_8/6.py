from numpy.random import randint
from numpy import sqrt, log2
import numpy as np
import os
from tqdm import tqdm
from matplotlib import pyplot as plt

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


MAX = 100000

C_list = list(range(10, 1000, 20))
t_list = []
for C in tqdm(C_list):
    t_sum = 0.0
    for i in range(MAX):
        x = randint(C)
        _, t = measure_time(primes, [x])
        t_sum += t
    t_sum /= MAX
    t_list.append(t_sum)

t_list = normalize_by_last(t_list)

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

val_to_normalize = np.max([t_list, t_list_1, t_list_2, t_list_3, t_list_4, t_list_5])

t_list = [e * 0.01 for e in t_list]
t_list_1 = normalize_by_last(t_list_1, val_to_normalize)
t_list_2 = normalize_by_last(t_list_2, val_to_normalize)
t_list_3 = normalize_by_last(t_list_3, val_to_normalize)
t_list_4 = normalize_by_last(t_list_4, val_to_normalize)
t_list_5 = normalize_by_last(t_list_5, val_to_normalize)

plt.figure()
plt.plot(C_list, t_list, color='black', markersize=5, marker='o', linewidth=2, linestyle='solid', label='experiment')
plt.plot(C_list, t_list_1, linewidth=2, linestyle='solid', label='$\sqrt{C} + \log(C)$')
plt.plot(C_list, t_list_2, linewidth=2, linestyle='solid', label='$\sqrt{C} \log(C)$')
plt.plot(C_list, t_list_3, linewidth=2, linestyle='solid', label='$\log(C)$')
plt.plot(C_list, t_list_4, linewidth=2, linestyle='solid', label='$\sqrt{C}$')
plt.plot(C_list, t_list_5, linewidth=2, linestyle='solid', label='$C$')
plt.grid(linewidth=1, linestyle='dotted', color='gray', alpha=0.5)
plt.ylim([0, 0.05])
plt.xlabel('C', fontweight='bold')
plt.ylabel('t, a.u.', fontweight='bold')
plt.legend()
path_to_save = '/'.join(os.path.abspath(__file__).split('.')[:-1]) + '.png'
plt.savefig(path_to_save , bbox_inches='tight')
plt.close()
