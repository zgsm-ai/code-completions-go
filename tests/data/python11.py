def fibonacci(n):
    if n <= 1:
        return n
    else:
        return fibonacci(n-1) + fibonacci(n-2)

def main():
    n = 10
    result = fibonacci(n)
    print(f"Fibonacci({n}) = {result}")<｜fim▁hole｜>

if __name__ == "__main__":
    main()