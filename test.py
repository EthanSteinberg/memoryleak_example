import helloworld as m
import gc
import weakref

class NobodyCares:
    def __init__(self):
        pass

def simple_test(f):
    it = NobodyCares()
    a = weakref.ref(it)
    try:
        f(it)
    except Exception as e:
        print("Got exception!", e)
        del e

    assert a() is not None
    del it
    gc.collect()
    assert a() is None

simple_test(m.custom_fancy_func)
simple_test(m.fancy_func)