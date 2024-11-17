def KnapSack(C, items):
    K = [[0 for _ in range(C+1)] for _ in range(len(items))]
    
    for i in range(0, len(items)):
        for w in range(0, C+1):
            if(items[i][0] > w):
                K[i][w] = K[i-1][w]
            else:
                K[i][w] = max(K[i-1][w], K[i-1][w-items[i][0]]+items[i][1])
                
    return K, K[len(items)-1][C]

def printKnapSack(C, items, K):
    print("   배낭 용량  w  ", end="  ")
    for i in range(C+1):
        print(f"{i:2d}", end="  ")
    print()
    
    print("  물건 ", end=""); print("가치 ", end=""); print(" 무게 ")
    for i in range(len(items)):
        print(f" {i:4d} {items[i][1]:4d} {items[i][0]:4d} ", end=" ")
        for j in range(C+1):
            print(f"{K[i][j]:4d}", end="")
        print()

def main():
    items = [[5, 10], [4, 40], [6, 30], [3, 50]]
    C = 10
    K, result = KnapSack(C, items)
    printKnapSack(C, items, K)
    
if __name__ == "__main__":
    main()