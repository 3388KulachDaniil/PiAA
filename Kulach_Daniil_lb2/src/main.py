from collections import deque
import string

class TrieNode:
    def __init__(self):
        self.children = dict()
        self.suffix_link = None
        self.output_link = None
        self.pattern_ids = []
        self.index = 0

class AhoCorasick:
    def __init__(self):
        self.root = TrieNode()
        self.root.index = 0
        self.root.suffix_link = self.root
        self.nodes = [self.root]
        self.patterns = []

    def add_pattern(self, pattern, pattern_id, wildcard=False):
        print(f"  Inserting pattern '{pattern}' (ID {pattern_id})")
        self._insert(self.root, pattern, 0, pattern_id, wildcard)

    def _insert(self, node, pattern, pos, pattern_id, wildcard):
        if pos == len(pattern):
            node.pattern_ids.append(pattern_id)
            print(f"    Pattern {pattern_id} ends at node {node.index}")
            return
        c = pattern[pos]
        chars = string.ascii_uppercase if wildcard and c == '?' else [c]
        for ch in chars:
            if ch not in node.children:
                new_node = TrieNode()
                new_node.index = len(self.nodes)
                self.nodes.append(new_node)
                node.children[ch] = new_node
                print(f"    Creating new node {new_node.index} for character '{ch}'")
            print(f"    Moved to node {node.children[ch].index}")
            self._insert(node.children[ch], pattern, pos + 1, pattern_id, wildcard)

    def build_links(self):
        print("\nBuilding suffix links:")
        queue = deque()
        for child in self.root.children.values():
            child.suffix_link = self.root
            queue.append(child)
        while queue:
            current = queue.popleft()
            for c, child in current.children.items():
                fallback = current.suffix_link
                while fallback != self.root and c not in fallback.children:
                    fallback = fallback.suffix_link
                child.suffix_link = fallback.children.get(c, self.root)
                child.output_link = child.suffix_link if child.suffix_link.pattern_ids else child.suffix_link.output_link
                print(f"  Set suffix link for node {child.index} -> {child.suffix_link.index}")
                queue.append(child)
        for node in self.nodes[1:]:
            print(f"  Terminal link for node {node.index} -> {node.output_link.index if node.output_link else 'nil'}")
        print()

    def print_structure(self):
        print("Automaton structure:")
        for node in self.nodes:
            print(f"Node {node.index}:")
            print(f"  Suffix link: {node.suffix_link.index if node.suffix_link else 0}")
            print(f"  Terminal link: {node.output_link.index if node.output_link else 'nil'}")
            for c, child in node.children.items():
                print(f"  Transition '{c}' -> {child.index}")
            if node.pattern_ids:
                print(f"  Patterns: {node.pattern_ids}")
        print()

    def search(self, text, non_overlapping=False):
        print("\nSearch process:\n")
        node = self.root
        results = []
        last_end = -1

        for i, c in enumerate(text):
            pos = i + 1
            print(f"Processing character '{c}' at position {pos}")
            print(f"Current node: {node.index}")
            while node != self.root and c not in node.children:
                print(f"  Following suffix link {node.index} -> {node.suffix_link.index}")
                node = node.suffix_link
            if c in node.children:
                node = node.children[c]
                print(f"  Moved to node {node.index}")
            else:
                node = self.root

            temp = node
            while temp:
                if temp.pattern_ids:
                    for pid in temp.pattern_ids:
                        pat_len = len(self.patterns[pid - 1])
                        start = pos - pat_len
                        if not non_overlapping or start > last_end:
                            print(f"  Found pattern(s) {temp.pattern_ids} at position {start + 1}")
                            results.append((start + 1, pid))
                            if non_overlapping:
                                last_end = pos - 1
                            break
                temp = temp.output_link
        return results

def wildcard_search(text, pattern):
    n, m = len(text), len(pattern)
    results = []

    for i in range(n - m + 1):
        match = True
        for j in range(m):
            if pattern[j] != '?' and pattern[j] != text[i + j]:
                match = False
                break
        if match:
            results.append(i + 1)
    return results

# ------------------- Интерфейс -------------------
print("Choose task:")
print("1. Multiple pattern search")
print("2. Wildcard pattern search")
print("3. Non-overlapping pattern search")
choice = input().strip()

if choice == "2":
    text = input().strip()
    pattern = input().strip()
    results = wildcard_search(text, pattern)
    print("\nOutput")
    if results:
        for r in results:
            print(r)
    else:
        print(-1)

else:
    print("Enter text:", end=" ")
    text = input().strip()

    print("Number of patterns:", end=" ")
    n = int(input().strip())

    patterns = []
    for i in range(n):
        print(f"Pattern {i + 1}:", end=" ")
        patterns.append(input().strip())

    print("\nBuilding trie:")
    automaton = AhoCorasick()
    automaton.patterns = patterns
    wildcard = choice == "2"
    for i, pat in enumerate(patterns):
        automaton.add_pattern(pat, i + 1, wildcard=wildcard)

    automaton.build_links()
    automaton.print_structure()

    non_overlapping = choice == "3"
    results = automaton.search(text, non_overlapping=non_overlapping)

    print("\nOutput")
    if choice == "1":
        for pos, pid in results:
            print(f"{pos} {pid}")
    elif choice == "3":
        for pos, _ in results:
            print(pos)
    else:
        print("-1" if not results else " ".join(str(pos) for pos, _ in results))
