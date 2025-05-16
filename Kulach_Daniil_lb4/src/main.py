def compute_prefix_function_verbose(P):
    print("Построение pi-функции:")
    n = len(P)
    pi = [0] * n
    j = 0
    for i in range(1, n):
        while j > 0 and P[i] != P[j]:
            print(f"q={i}: {P[i]}!={P[j]} и k={j}; переход к pi[{j - 1}]={pi[j - 1]}")
            j = pi[j - 1]
        if P[i] == P[j]:
            j += 1
            print(f"q={i}: {P[i]}=={P[j-1]}; pi[{i}]={j}")
        else:
            print(f"q={i}: {P[i]}!={P[j]} и k=0; pi[{i}]=0")
        pi[i] = j
    print(f"Результат pi: {pi}\n")
    return pi

def kmp_search_verbose(P, T):
    print("Поиск вхождений:")
    pi = compute_prefix_function_verbose(P)
    result = []
    j = 0
    for i in range(len(T)):
        while j > 0 and T[i] != P[j]:
            print(f"i={i}: {T[i]}!={P[j]}, q={j} -> q={pi[j - 1]}")
            j = pi[j - 1]
        if T[i] == P[j]:
            j += 1
            print(f"i={i}: {T[i]} совпало, q-> {j}")
        else:
            print(f"i={i}: {T[i]}!={P[j]}, q={j}")
        if j == len(P):
            print(f"Найдено вхождение с {i - len(P) + 1}")
            result.append(i - len(P) + 1)
            j = pi[j - 1]
    return result

def kmp_search_shift_verbose(P, T):
    print("Проверка циклического сдвига:")
    pi = compute_prefix_function_verbose(P)
    j = 0
    for i in range(len(T)):
        while j > 0 and T[i] != P[j]:
            print(f"i={i}: {T[i]}!={P[j]}, q={j} -> q={pi[j - 1]}")
            j = pi[j - 1]
        if T[i] == P[j]:
            j += 1
            print(f"i={i}: {T[i]} совпало, q-> {j}")
        else:
            print(f"i={i}: {T[i]}!={P[j]}, q={j}")
        if j == len(P):
            print(f"Найден сдвиг {i - len(P) + 1}")
            return i - len(P) + 1
    return -1

def main():
    task = input("").strip()

    if task == "1":
        P = input().strip()
        T = input().strip()
        matches = kmp_search_verbose(P, T)
        if matches:
            print(",".join(map(str, matches)))
        else:
            print(-1)

    elif task == "2":
        A = input().strip()
        B = input().strip()
        if len(A) != len(B):
            print(-1)
        else:
            A2 = A + A
            result = kmp_search_shift_verbose(B, A2)
            print(result)

    else:
        print("Некорректный выбор. Введите 1 или 2.")

if __name__ == "__main__":
    main()
