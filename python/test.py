class Test:
    def add(arg1, arg2):
        return arg1 + arg2

class Foo:
    def __init__(self):
        self.internal = Test()

    def add(self):
        return self.internal.add


foo = Foo()

print(foo.add().__func__(1, 2))

