#include <cstddef>
#include <iostream>
#include <stdexcept>

using namespace std;

template <typename T>
class DoublyLinkedList {
private:
    class Node {
    public:
        T value;
        Node* prev;
        Node* next;

        explicit Node(const T& value)
            : value(value), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    size_t list_size;

   
    Node* node_at(size_t position) const {
        if (position >= list_size) {
            throw out_of_range("Position is outside the list");
        }

        if (position < list_size / 2) {
            Node* current = head;
            for (size_t i = 0; i < position; ++i) {
                current = current->next;
            }
            return current;
        }

        Node* current = tail;
        for (size_t i = list_size - 1; i > position; --i) {
            current = current->prev;
        }
        return current;
    }

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), list_size(0) {}

    // Запрещаем неявное копирование, чтобы два списка не владели одними узлами.
    DoublyLinkedList(const DoublyLinkedList&) = delete;
    DoublyLinkedList& operator=(const DoublyLinkedList&) = delete;

    ~DoublyLinkedList() {
        clear();
    }

    void push_front(const T& value) {
        Node* new_node = new Node(value);
        new_node->next = head;

        if (head != nullptr) {
            head->prev = new_node;
        } else {
            tail = new_node;
        }

        head = new_node;
        ++list_size;
    }

    void push_back(const T& value) {
        Node* new_node = new Node(value);
        new_node->prev = tail;

        if (tail != nullptr) {
            tail->next = new_node;
        } else {
            head = new_node;
        }

        tail = new_node;
        ++list_size;
    }

    void pop_front() {
        if (empty()) {
            throw out_of_range("Cannot pop_front from an empty list");
        }

        Node* old_head = head;
        head = head->next;

        if (head != nullptr) {
            head->prev = nullptr;
        } else {
            tail = nullptr;
        }

        delete old_head;
        --list_size;
    }

    void pop_back() {
        if (empty()) {
            throw out_of_range("Cannot pop_back from an empty list");
        }

        Node* old_tail = tail;
        tail = tail->prev;

        if (tail != nullptr) {
            tail->next = nullptr;
        } else {
            head = nullptr;
        }

        delete old_tail;
        --list_size;
    }

    void insert(size_t position, const T& value) {
        if (position > list_size) {
            throw out_of_range("Insert position is outside the list");
        }
        if (position == 0) {
            push_front(value);
            return;
        }
        if (position == list_size) {
            push_back(value);
            return;
        }

        Node* next_node = node_at(position);
        Node* prev_node = next_node->prev;
        Node* new_node = new Node(value);

        new_node->prev = prev_node;
        new_node->next = next_node;
        prev_node->next = new_node;
        next_node->prev = new_node;
        ++list_size;
    }

    void erase(size_t position) {
        if (position >= list_size) {
            throw out_of_range("Erase position is outside the list");
        }
        if (position == 0) {
            pop_front();
            return;
        }
        if (position == list_size - 1) {
            pop_back();
            return;
        }

        Node* node_to_delete = node_at(position);
        node_to_delete->prev->next = node_to_delete->next;
        node_to_delete->next->prev = node_to_delete->prev;
        delete node_to_delete;
        --list_size;
    }

    ptrdiff_t find(const T& value) const {
        Node* current = head;
        ptrdiff_t position = 0;

        while (current != nullptr) {
            if (current->value == value) {
                return position;
            }
            current = current->next;
            ++position;
        }
        return -1;
    }

    void clear() {
        while (head != nullptr) {
            Node* node_to_delete = head;
            head = head->next;
            delete node_to_delete;
        }
        tail = nullptr;
        list_size = 0;
    }

    size_t size() const {
        return list_size;
    }

    bool empty() const {
        return list_size == 0;
    }

    void print_forward() const {
        Node* current = head;
        while (current != nullptr) {
            cout << current->value;
            if (current->next != nullptr) {
                cout << ' ';
            }
            current = current->next;
        }
        cout << '\n';
    }

    void print_backward() const {
        Node* current = tail;
        while (current != nullptr) {
            cout << current->value;
            if (current->prev != nullptr) {
                cout << ' ';
            }
            current = current->prev;
        }
        cout << '\n';
    }
};

int main() {
    DoublyLinkedList<int> numbers;

    numbers.push_back(10);
    numbers.push_back(20);
    numbers.push_front(5);
    numbers.insert(2, 15);

    cout << "Forward: ";
    numbers.print_forward();       // 20 15 10 5

    cout << "Backward: ";
    numbers.print_backward();      // 5 10 15 20

    cout << "Index of 15: " << numbers.find(15) << '\n';
    cout << "Size: " << numbers.size() << '\n';

    numbers.erase(1);
    numbers.pop_front();
    numbers.pop_back();

    cout << "After deletion: ";
    numbers.print_forward();       // 15

    numbers.clear();
    cout << "Is empty: " << boolalpha << numbers.empty() << '\n';

    return 0;
}
