import sys

def fib(count):
    prev = 0
    curr = 1

    for i in range(0, count):
        tmp = curr
        curr = curr + prev
        prev = tmp

    return prev

def main():
    if (len(sys.argv) < 1):
        print("You need to supply an argument")
        sys.exit(1)

    count = int(sys.argv[1])

    if (count < 0):
        print("You need to use a positve number")
        sys.exit(1)

    result = fib(count)

    print(result)

if __name__ == "__main__":
    main()
