#ifndef HISTORY_LIST_H
#define HISTORY_LIST_H

#include <string>
#include <vector>

/**
 * @brief Node for the Doubly Linked List storing calculation history.
 * 
 * DSA Concept:
 * A Doubly Linked List is a linear data structure where each node contains
 * data and two pointers: one pointing to the next node and one to the previous
 * node. This allows efficient bidirectional traversal and O(1) insertions at
 * either head or tail without reallocating arrays.
 */
struct HistoryNode {
    std::string expression;  ///< Original input expression (e.g. "2 + 3 * 4")
    std::string result;      ///< Calculated result (e.g. "14")
    HistoryNode* prev;       ///< Pointer to previous node
    HistoryNode* next;       ///< Pointer to next node

    HistoryNode(const std::string& expr, const std::string& res)
        : expression(expr), result(res), prev(nullptr), next(nullptr) {}
};

/**
 * @brief Custom Doubly Linked List specifically tailored for Calculator History.
 */
class HistoryList {
private:
    HistoryNode* head;  ///< Pointer to the oldest calculation entry
    HistoryNode* tail;  ///< Pointer to the newest calculation entry
    int count;          ///< Total number of history entries stored
    int maxCapacity;    ///< Optional limit to prevent unbounded memory growth

public:
    /**
     * @brief Constructs an empty HistoryList.
     * @param maxCap Maximum entries to keep (default 100).
     */
    explicit HistoryList(int maxCap = 100);

    /**
     * @brief Destructor frees all dynamically allocated nodes.
     */
    ~HistoryList();

    // Prevent copying for simplicity and clean memory ownership
    HistoryList(const HistoryList&) = delete;
    HistoryList& operator=(const HistoryList&) = delete;

    /**
     * @brief Appends a new calculation record to the history.
     * Time Complexity: O(1) - inserts directly at tail.
     */
    void add(const std::string& expression, const std::string& result);

    /**
     * @brief Removes the oldest record from the head if capacity is exceeded.
     * Time Complexity: O(1)
     */
    void removeOldest();

    /**
     * @brief Clears all entries from the history list.
     * Time Complexity: O(N) where N is number of entries.
     */
    void clear();

    /**
     * @brief Returns the number of items stored.
     */
    int size() const;

    /**
     * @brief Returns true if history contains no entries.
     */
    bool isEmpty() const;

    /**
     * @brief Returns all history entries formatted from newest to oldest.
     * e.g., ["2 + 3 * 4 = 14", "(10 + 2) * 4 = 48"]
     */
    std::vector<std::string> getFormattedEntries() const;

    /**
     * @brief Retrieves expression and result at a specific 0-based index (0 = newest).
     */
    bool getEntry(int indexFromNewest, std::string& outExpr, std::string& outResult) const;
};

#endif // HISTORY_LIST_H
