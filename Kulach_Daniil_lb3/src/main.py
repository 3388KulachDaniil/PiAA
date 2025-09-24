import tkinter as tk
from tkinter import ttk, messagebox, filedialog
import csv

INF = float('inf')


def initialize_dp(n, m, delete_cost, insert_cost):
    D = [[INF] * (m + 1) for _ in range(n + 1)]
    D[0][0] = 0
    for i in range(1, n + 1):
        D[i][0] = D[i-1][0] + delete_cost
    for j in range(1, m + 1):
        D[0][j] = D[0][j-1] + insert_cost
    return D


def fill_dp(D, A, B, replace_cost, insert_cost, delete_cost, delete_two_cost):
    n = len(A)
    m = len(B)
    for i in range(n + 1):
        for j in range(m + 1):
            if i > 0:
                D[i][j] = min(D[i][j], D[i-1][j] + delete_cost)
            if j > 0:
                D[i][j] = min(D[i][j], D[i][j-1] + insert_cost)
            if i > 0 and j > 0:
                cost = 0 if A[i-1] == B[j-1] else replace_cost
                D[i][j] = min(D[i][j], D[i-1][j-1] + cost)
            if i >= 2 and A[i-1] != A[i-2]:
                D[i][j] = min(D[i][j], D[i-2][j] + delete_two_cost)


def backtrack_operations(D, A, B, replace_cost, insert_cost, delete_cost, delete_two_cost):
    operations = []
    i = len(A)
    j = len(B)

    log_lines = []
    log_lines.append("Пошаговое восстановление операций:\n(Ищем путь от правого нижнего угла к началу матрицы)")

    while i > 0 or j > 0:
        a_char = A[i-1] if i > 0 else ' '
        b_char = B[j-1] if j > 0 else ' '
        log_lines.append(f"\nПозиция: A[{i}]='{a_char}', B[{j}]='{b_char}'")
        log_lines.append(f"Текущая стоимость: {D[i][j]}")

        if i >= 2 and D[i][j] == D[i-2][j] + delete_two_cost and A[i-1] != A[i-2]:
            log_lines.append(f"[DT]: удаление двух разных символов '{A[i-2]}{A[i-1]}'")
            operations.append('[DT]')
            i -= 2
        elif j > 0 and D[i][j] == D[i][j-1] + insert_cost:
            log_lines.append(f"I: вставка '{B[j-1]}'")
            operations.append('I')
            j -= 1
        elif i > 0 and D[i][j] == D[i-1][j] + delete_cost:
            log_lines.append(f"D: удаление '{A[i-1]}'")
            operations.append('D')
            i -= 1
        else:
            if i > 0 and j > 0 and A[i-1] == B[j-1]:
                log_lines.append(f"M: совпадение '{A[i-1]}'")
                operations.append('M')
            else:
                log_lines.append(f"R: замена '{A[i-1] if i>0 else ''}' на '{B[j-1] if j>0 else ''}'")
                operations.append('R')
            i -= 1
            j -= 1
        log_lines.append(f"Текущие операции: {list(reversed(operations))}")

    operations.reverse()
    return operations, "\n".join(log_lines)


class EditDistanceGUI(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Редактор расстояния (c удалением пары) — GUI")
        self.geometry("1100x700")
        self._build_widgets()

    def _build_widgets(self):
        top = ttk.LabelFrame(self, text="Параметры")
        top.pack(fill=tk.X, padx=10, pady=10)

        grid = ttk.Frame(top)
        grid.pack(fill=tk.X, padx=10, pady=10)

        self.var_replace = tk.StringVar(value="1")
        self.var_insert = tk.StringVar(value="1")
        self.var_delete = tk.StringVar(value="1")
        self.var_delete2 = tk.StringVar(value="2")

        self.var_A = tk.StringVar(value="")
        self.var_B = tk.StringVar(value="")

        labels = [
            ("Стоимость замены", self.var_replace),
            ("Стоимость вставки", self.var_insert),
            ("Стоимость удаления", self.var_delete),
            ("Удаление двух разных подряд", self.var_delete2),
            ("Строка A", self.var_A),
            ("Строка B", self.var_B),
        ]

        for r, (text, var) in enumerate(labels):
            ttk.Label(grid, text=text + ":").grid(row=r, column=0, sticky=tk.W, pady=3)
            entry = ttk.Entry(grid, textvariable=var, width=40)
            entry.grid(row=r, column=1, sticky=tk.W, pady=3)

        # Buttons
        btns = ttk.Frame(top)
        btns.pack(fill=tk.X, padx=10, pady=(0,10))
        ttk.Button(btns, text="Вычислить", command=self.compute).pack(side=tk.LEFT)
        ttk.Button(btns, text="Очистить", command=self.clear).pack(side=tk.LEFT, padx=6)
        ttk.Button(btns, text="Экспорт матрицы в CSV", command=self.export_csv).pack(side=tk.LEFT)

        self.nb = ttk.Notebook(self)
        self.nb.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)

        self.tab_matrix = ttk.Frame(self.nb)
        self.nb.add(self.tab_matrix, text="Матрица")
        self._build_matrix_tab()

        self.tab_log = ttk.Frame(self.nb)
        self.nb.add(self.tab_log, text="Лог восстановления")
        self._build_log_tab()

        self.tab_result = ttk.Frame(self.nb)
        self.nb.add(self.tab_result, text="Итог")
        self._build_result_tab()

        self.current_matrix = None
        self.current_A = ""
        self.current_B = ""

    def _build_matrix_tab(self):
        container = ttk.Frame(self.tab_matrix)
        container.pack(fill=tk.BOTH, expand=True)

        self.tree = ttk.Treeview(container, show='headings', height=20)
        vsb = ttk.Scrollbar(container, orient="vertical", command=self.tree.yview)
        hsb = ttk.Scrollbar(container, orient="horizontal", command=self.tree.xview)
        self.tree.configure(yscrollcommand=vsb.set, xscrollcommand=hsb.set)

        self.tree.grid(row=0, column=0, sticky="nsew")
        vsb.grid(row=0, column=1, sticky="ns")
        hsb.grid(row=1, column=0, sticky="ew")

        container.rowconfigure(0, weight=1)
        container.columnconfigure(0, weight=1)

    def _build_log_tab(self):
        self.txt_log = tk.Text(self.tab_log, wrap='word')
        self.txt_log.pack(fill=tk.BOTH, expand=True)

    def _build_result_tab(self):
        frm = ttk.Frame(self.tab_result)
        frm.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)

        self.var_ops = tk.StringVar(value="")
        self.var_cost = tk.StringVar(value="")

        ttk.Label(frm, text="Операции:").grid(row=0, column=0, sticky=tk.W)
        ttk.Entry(frm, textvariable=self.var_ops, width=100).grid(row=0, column=1, sticky=tk.W)

        ttk.Label(frm, text="Итоговая стоимость:").grid(row=1, column=0, sticky=tk.W, pady=(8,0))
        ttk.Entry(frm, textvariable=self.var_cost, width=20).grid(row=1, column=1, sticky=tk.W, pady=(8,0))

        ttk.Label(frm, text="A:").grid(row=2, column=0, sticky=tk.W, pady=(12,0))
        self.lbl_A = ttk.Entry(frm, width=100)
        self.lbl_A.grid(row=2, column=1, sticky=tk.W, pady=(12,0))

        ttk.Label(frm, text="B:").grid(row=3, column=0, sticky=tk.W)
        self.lbl_B = ttk.Entry(frm, width=100)
        self.lbl_B.grid(row=3, column=1, sticky=tk.W)

    def _parse_int(self, s, name):
        try:
            return int(s)
        except ValueError:
            raise ValueError(f"'{name}' должно быть целым числом")

    def compute(self):
        try:
            replace_cost = self._parse_int(self.var_replace.get(), "Стоимость замены")
            insert_cost = self._parse_int(self.var_insert.get(), "Стоимость вставки")
            delete_cost = self._parse_int(self.var_delete.get(), "Стоимость удаления")
            delete_two_cost = self._parse_int(self.var_delete2.get(), "Удаление двух подряд")
            A = self.var_A.get()
            B = self.var_B.get()

            n, m = len(A), len(B)
            D = initialize_dp(n, m, delete_cost, insert_cost)
            fill_dp(D, A, B, replace_cost, insert_cost, delete_cost, delete_two_cost)

            operations, log_text = backtrack_operations(
                D, A, B, replace_cost, insert_cost, delete_cost, delete_two_cost
            )

            self._populate_matrix(D, A, B)
            self.txt_log.delete('1.0', tk.END)
            self.txt_log.insert(tk.END, log_text)

            self.var_ops.set(''.join(operations))
            self.var_cost.set(str(D[len(A)][len(B)]))
            self.lbl_A.delete(0, tk.END)
            self.lbl_A.insert(0, A)
            self.lbl_B.delete(0, tk.END)
            self.lbl_B.insert(0, B)

            self.current_matrix = D
            self.current_A, self.current_B = A, B
            self.nb.select(self.tab_matrix)
        except Exception as e:
            messagebox.showerror("Ошибка", str(e))

    def _populate_matrix(self, D, A, B):
        for col in self.tree['columns']:
            self.tree.heading(col, text='')
        self.tree.delete(*self.tree.get_children())

        n = len(A)
        m = len(B)

        columns = ["A/B"] + [f"{j}:{B[j-1]}" for j in range(1, m+1)]
        self.tree['columns'] = columns

        for col in columns:
            self.tree.heading(col, text=col)
            self.tree.column(col, width=90, anchor=tk.CENTER)

        for i in range(0, n+1):
            row_label = f"{i}:{A[i-1]}" if i > 0 else "0:∅"
            row_vals = []
            for j in range(0, m+1):
                val = D[i][j]
                row_vals.append("∞" if val == INF else str(int(val)))
            self.tree.insert('', tk.END, values=[row_label] + row_vals)

    def export_csv(self):
        if self.current_matrix is None:
            messagebox.showinfo("Экспорт", "Сначала вычислите матрицу.")
            return
        path = filedialog.asksaveasfilename(
            defaultextension=".csv",
            filetypes=[("CSV", ".csv")],
            title="Сохранить матрицу как…"
        )
        if not path:
            return
        try:
            with open(path, 'w', newline='', encoding='utf-8') as f:
                writer = csv.writer(f)
                B = self.current_B
                header = ["A/B"] + [b for b in ("∅" + B)]
                writer.writerow(header)
                A = self.current_A
                for i, row in enumerate(self.current_matrix):
                    row_label = "∅" if i == 0 else A[i-1]
                    writer.writerow([row_label] + ["∞" if v == INF else int(v) for v in row])
            messagebox.showinfo("Экспорт", f"Матрица сохранена в {path}")
        except Exception as e:
            messagebox.showerror("Ошибка экспорта", str(e))

    def clear(self):
        self.var_replace.set("1")
        self.var_insert.set("1")
        self.var_delete.set("1")
        self.var_delete2.set("2")
        self.var_A.set("")
        self.var_B.set("")
        self.var_ops.set("")
        self.var_cost.set("")
        self.txt_log.delete('1.0', tk.END)
        self.current_matrix = None
        self.current_A = self.current_B = ""
        for col in self.tree['columns']:
            self.tree.heading(col, text='')
        self.tree.delete(*self.tree.get_children())


if __name__ == '__main__':
    app = EditDistanceGUI()
    app.mainloop()
