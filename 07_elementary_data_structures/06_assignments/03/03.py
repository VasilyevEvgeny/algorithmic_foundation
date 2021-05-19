class Enterprises:
    def __init__(self, **kwargs):
        self.__n = kwargs['n']
        self.__a = kwargs['a']
        self.__k = kwargs['k']
        self.__ev = kwargs['ev']

        self.__enterprises = []
        self.__sum_of_squares = 0
        for e in a:
            self.__enterprises.append(e)
            self.__sum_of_squares += e**2

        self.__verbose = kwargs.get('verbose', False)

        if self.__verbose:
            print('#######\nINITIAL:\nn = {}\na = {}\nk = {}\nev = {}\nenterprises = {}\n#######'.format(self.__n,
                                                                                                         self.__a,
                                                                                                         self.__k,
                                                                                                         self.__ev,
                                                                                                         self.__enterprises))

    def __calculate_portions(self, unit):
        first = unit // 2
        second = unit - first
        if self.__verbose:
            print('first = {}\nsecond = {}'.format(first, second))

        return first, second

    def __divide(self, j):
        if self.__verbose:
            print('go divide! j = {}'.format(j))

        portions = self.__calculate_portions(self.__enterprises[j - 1])

        # add second portion
        self.__enterprises.insert(j - 1, portions[1])
        self.__sum_of_squares += portions[1]**2
        if self.__verbose:
            print('add second portion squared, sum_of_squares --> {}'.format(self.__sum_of_squares))

        # add first portion
        self.__enterprises.insert(j - 1, portions[0])
        self.__sum_of_squares += portions[0]**2
        if self.__verbose:
            print('add first portion squared, sum_of_squares --> {}'.format(self.__sum_of_squares))

        # erase initial element
        self.__sum_of_squares -= self.__enterprises[j+1]**2
        if self.__verbose:
            print('remove initial squared, sum_of_squares --> {}'.format(self.__sum_of_squares))
        self.__enterprises.pop(j + 1)

    def __bankrupt(self, j):
        if self.__verbose:
            print('go bankrupt! j = {}'.format(j))

        if j == 1:
            self.__sum_of_squares -= self.__enterprises[0]**2
            self.__sum_of_squares -= self.__enterprises[1]**2
            if self.__verbose:
                print('remove cur and next squared, sum_of_squares --> {}'.format(self.__sum_of_squares))
            self.__enterprises[1] += self.__enterprises[0]
            self.__sum_of_squares += self.__enterprises[1]**2
            if self.__verbose:
                print('add next squared, sum_of_squares --> {}'.format(self.__sum_of_squares))
            self.__enterprises.pop(0)

        elif j == len(self.__enterprises):
            size = len(self.__enterprises) - 1
            self.__sum_of_squares -= self.__enterprises[size]**2
            self.__sum_of_squares -= self.__enterprises[size - 1]**2
            if self.__verbose:
                print('remove last and pre-last, sum_of_squares --> {}'.format(self.__sum_of_squares))
            self.__enterprises[size - 1] += self.__enterprises[size]
            self.__sum_of_squares += self.__enterprises[size - 1]**2
            if self.__verbose:
                print('add pre-last squared, sum_of_squares --> {}'.format(self.__sum_of_squares))
            self.__enterprises.pop(size)

        else:
            portions = self.__calculate_portions(self.__enterprises[j - 1])

            # cur
            self.__sum_of_squares -= self.__enterprises[j - 1]**2
            if self.__verbose:
                print('remove cur squared, sum_of_squares --> {}'.format(self.__sum_of_squares))

            # prev
            self.__sum_of_squares -= self.__enterprises[j - 2]**2
            if self.__verbose:
                print('remove prev squared, sum_of_squares --> {}'.format(self.__sum_of_squares))
            self.__enterprises[j - 2] += portions[0]
            self.__sum_of_squares += self.__enterprises[j - 2]**2
            if self.__verbose:
                print('add prev squared, sum_of_squares --> {}'.format(self.__sum_of_squares))

            # next
            self.__sum_of_squares -= self.__enterprises[j]**2
            if self.__verbose:
                print('remove next squared, sum_of_squares --> {}'.format(self.__sum_of_squares))
            self.__enterprises[j] += portions[1]
            self.__sum_of_squares += self.__enterprises[j]**2
            if self.__verbose:
                print('add next squared, sum_of_squares --> {}'.format(self.__sum_of_squares))

            self.__enterprises.pop(j - 1)

    def make_evolution(self):
        print(self.__sum_of_squares)

        for item in ev:
            if item['e'] == 1:
                self.__bankrupt(item['v'])
            elif item['e'] == 2:
                self.__divide(item['v'])

            print(self.__sum_of_squares)
            if self.__verbose:
                print('enterprises = {}\n{}'.format(self.__enterprises,
                                                    '===================================================='))


n = int(input())
a = [int(e) for e in input().split(' ')]

k = int(input())
ev = []
for i in range(k):
    line = input().split()
    e, v = int(line[0]), int(line[1])
    ev.append({'e': e, 'v': v})

enterprises = Enterprises(n=n, a=a, k=k, ev=ev, verbose=False)
enterprises.make_evolution()
