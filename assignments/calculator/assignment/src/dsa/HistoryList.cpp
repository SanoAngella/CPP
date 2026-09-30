#include "HistoryList.h"

HistoryList::HistoryList(int maxCap)
    : head(nullptr), tail(nullptr), count(0), maxCapacity(maxCap > 0 ? maxCap : 100) {}

HistoryList::~HistoryList() {
    clear();
}

void HistoryList::add(const std::string& expression, const std::string& result) {
    HistoryNode* newNode = new HistoryNode(expression, result);

    if (isEmpty()) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    count++;

    if (count > maxCapacity) {
        removeOldest();
    }
}

void HistoryList::removeOldest() {
    if (isEmpty()) return;

    HistoryNode* temp = head;
    head = head->next;
    if (head) {
        head->prev = nullptr;
    } else {
        tail = nullptr;
    }
    delete temp;
    count--;
}

void HistoryList::clear() {
    HistoryNode* current = head;
    while (current != nullptr) {
        HistoryNode* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head = nullptr;
    tail = nullptr;
    count = 0;
}

int HistoryList::size() const {
    return count;
}

bool HistoryList::isEmpty() const {
    return count == 0;
}

std::vector<std::string> HistoryList::getFormattedEntries() const {
    std::vector<std::string> entries;
    // Traverse backwards from tail (newest) to head (oldest)
    HistoryNode* current = tail;
    while (current != nullptr) {
        entries.push_back(current->expression + " = " + current->result);
        current = current->prev;
    }
    return entries;
}

bool HistoryList::getEntry(int indexFromNewest, std::string& outExpr, std::string& outResult) const {
    if (indexFromNewest < 0 || indexFromNewest >= count) {
        return false;
    }
    HistoryNode* current = tail;
    for (int i = 0; i < indexFromNewest && current != nullptr; ++i) {
        current = current->prev;
    }
    if (current != nullptr) {
        outExpr = current->expression;
        outResult = current->result;
        return true;
    }
    return false;
}
