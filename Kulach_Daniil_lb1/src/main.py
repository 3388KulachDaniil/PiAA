def find_min_squares_with_debug(N, DEBUG=True):
    import sys
    sys.setrecursionlimit(10000)

    total_cells = N * N
    full_mask = (1 << total_cells) - 1

    squares_masks = {}
    for y in range(N):
        for x in range(N):
            max_side = min(N - x, N - y)
            for s in range(1, max_side + 1):
                if s == N:
                    continue
                mask = 0
                for r in range(y, y + s):
                    for c in range(x, x + s):
                        mask |= (1 << (r * N + c))
                squares_masks[(x, y, s)] = mask

    def find_first_free(board, start_idx):
        for i in range(start_idx, total_cells):
            if ((board >> i) & 1) == 0:
                return i
        return -1

    best_count = [float('inf')]
    best_solution = [[]]
    dp = {}
    solution = []
    iteration_counter = [0]

    def print_solution_matrix(n, result):
        print("\nИтоговая матрица решения")
        matrix = [[0] * n for _ in range(n)]
        num = 1
        for (x, y, s) in result:
            for i in range(s):
                for j in range(s):
                    matrix[y - 1 + i][x - 1 + j] = num
            num += 1

        for row in matrix:
            print(" ".join(f"{cell:2}" for cell in row))

    def search(board, current_count, start_idx, free_count):
        iteration_counter[0] += 1
        iter_num = iteration_counter[0]

        if DEBUG:
            print(f"\nИтерация #{iter_num}:")
            print(f"Текущий стек ({len(solution)} квадратов):")
            for sq in solution:
                print(f"\tКвадрат: ({sq[0]}, {sq[1]}) размер {sq[2]}")

        if current_count >= best_count[0]:
            if DEBUG:
                print("\tОбрезка ветви: текущих квадратов уже больше оптимума")
            return
        if board == full_mask:
            if current_count < best_count[0]:
                best_count[0] = current_count
                best_solution[0] = solution.copy()
                if DEBUG:
                    print(f"\tНайден новый лучший результат: {current_count} квадратов")
            return
        if board in dp and dp[board] <= current_count:
            return
        dp[board] = current_count

        idx = find_first_free(board, start_idx)
        if idx == -1:
            return
        x = idx % N
        y = idx // N

        max_possible = min(N - x, N - y)
        if max_possible == N:
            max_possible = N - 1
        if max_possible <= 0:
            max_possible = 1
        max_area = max_possible * max_possible
        lower_bound = (free_count + max_area - 1) // max_area
        if current_count + lower_bound >= best_count[0]:
            if DEBUG:
                print("\tОбрезка ветви: оценка нижней границы >= текущего оптимума")
            return

        max_side = min(N - x, N - y)
        if max_side == N:
            max_side = N - 1

        for s in range(max_side, 0, -1):
            mask = squares_masks.get((x, y, s))
            if mask is None:
                continue
            if board & mask == 0:
                solution.append((x + 1, y + 1, s))
                if DEBUG:
                    print(f"\tДобавляем квадрат: ({x + 1}, {y + 1}) размер {s}")
                search(board | mask, current_count + 1, idx + 1, free_count - s * s)
                if DEBUG:
                    print(f"\tУбираем квадрат: ({x + 1}, {y + 1}) размер {s}")
                solution.pop()

    search(0, 0, 0, total_cells)

    return best_count[0], best_solution[0], iteration_counter[0], print_solution_matrix


if __name__ == "__main__":
    N = int(input("Размер столешницы N: "))
    if 2 <= N < 20:
        count, squares, iterations, print_matrix_fn = find_min_squares_with_debug(N, DEBUG=True)
        print(f"\nМинимальное количество квадратов: {count}")
        for x, y, s in squares:
            print(f"{x} {y} {s}")
        print(f"\nКоличество итераций: {iterations}")
        print_matrix_fn(N, squares)
    else:
        print("Недопустимый размер. Введите число от 2 до 19.")
