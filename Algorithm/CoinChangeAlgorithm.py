def CoinChangeAlgorithm(num):
    n500 = 0
    n100 = 0
    n50 = 0
    n10 = 0

    while(num >= 500):
        num -= 500
        n500 += 1
    while(num >= 100):
        num -= 100
        n100 += 1
    while(num >= 50):
        num -= 50
        n50 += 1
    while(num >= 10):
        num -= 10
        n10 += 1

    return (n500, n100, n50, n10)

def main():
    try:
        # num = input("숫자를 입력하세요: ")
        num = 2780
        result = CoinChangeAlgorithm(int(num))

        print(f"{num} Won - 500 Won: {result[0]}, 100 Won: {result[1]}, 50 Won: {result[2]}, 10 Won: {result[3]}\n")

    except ValueError:
        print("유효한 숫자를 입력해 주세요.")


if __name__ == "__main__":
    main()