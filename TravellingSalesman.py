n = int(input())
A = [[int(j) for j in input().split()] for i in range(n)]
B = [[float("inf")] * n for i in range(1 << n)]
B[1][0] = 0
for mask in range(2, 1 << n):
    for i in range(n):
        if ((mask >> i) & 1):
            mask_without_i = mask ^ (1 << i)
            for j in range(n):
                if j != i and ((mask >> j) & 1):
                    B[mask][i] = min(B[mask][i],
                                     B[mask_without_i][j] + A[j][i])
mask_everything = (1 << n) - 1
answer = float("inf")
for i in range(n):
    answer = min(answer, B[mask_everything][i] + A[i][0])
print(answer)