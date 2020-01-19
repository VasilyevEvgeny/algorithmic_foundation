from time import time


def measure_time(function, args):
    """
    :param function: function object, the execution time of which must be measured
    :param args: arguments of that function
    :return: function result
    """
    t_start = time()
    res = function(*args)

    return res, time() - t_start


def normalize_by_last(input, val_to_normalize=None):
    if val_to_normalize is None:
        return [e / input[-1] for e in input]
    else:
        return [e / val_to_normalize for e in input]