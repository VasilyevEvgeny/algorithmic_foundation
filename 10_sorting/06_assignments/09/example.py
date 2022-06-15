def qsort(a, l, r):
    if l >= r:
        return

    x = a[(l + r) // 2]
    i = l
    j = r

    while i <= j:
        while a[i] < x:
            i += 1
        while a[j] > x:
            j -= 1

        if i <= j:
            a[i], a[j] = a[j], a[i]
            i += 1
            j -= 1

    qsort(a, l, j)
    qsort(a, i, r)


a = list(map(int, input().split()))
qsort(a, 0, len(a) - 1)
print(' '.join(map(str, a)))