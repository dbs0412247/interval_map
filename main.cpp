#include <iostream>
#include <vector>
#include <string>

#include <map>

/**
 * @brief
 *
 * @tparam K
 * Key type K
    - besides being copyable and assignable, is less-than comparable via operator<, and
    - does not implement any other operations, in particular no equality comparison or arithmetic operators.
 * @tparam V
 * Value type V
    - besides being copyable and assignable, is equality-comparable via operator==, and
    - does not implement any other operations.
 */

template<typename K, typename V>
class interval_map {
    //friend void IntervalMapTest();
    V m_valBegin;
    std::map<K,V> m_map;
public:
    // constructor associates whole range of K with val
    interval_map(V const& val)
    : m_valBegin(val)
    {}

    // Assign value val to interval [keyBegin, keyEnd).
    // Overwrite previous values in this interval.
    // Conforming to the C++ Standard Library conventions, the interval
    // includes keyBegin, but excludes keyEnd.
    // If !( keyBegin < keyEnd ), this designates an empty interval,
    // and assign must do nothing.
    void assign( K const& keyBegin, K const& keyEnd, V const& val ) {

        // Empty interval case
        if ( !( keyBegin < keyEnd ) ) {
            return;
        }

        bool isValEqValBegin = (val == m_valBegin);

        // In theory, the number of entries in m_map should be an non-negative integer N, with N != 1 (assuming no +ve infinity K).
        // i.e. N = 0, 2, 3, 4, ...

        // N == 0 case
        if ( m_map.empty() ) {

            // first internal in m_map
            if ( !isValEqValBegin ) {
                // insert the beginning and end m_map entries
                m_map[keyBegin] = val;
                m_map[keyEnd] = m_valBegin;
            }
            // Nothing needs to be done for the else case:
            // i.e. if the m_map is empty and the range assigned has equal value to m_valBegin.

            return;
        }

        auto itEnd = m_map.upper_bound(keyEnd);
        V valEnd = (itEnd == m_map.begin()? m_valBegin : (std::prev(itEnd))->second);

        if (itEnd == m_map.begin()) // both itBegin and itEnd are before the first element
        {
            if  (!isValEqValBegin) {
                m_map.insert_or_assign(keyBegin, val);
                m_map.insert_or_assign(keyEnd, m_valBegin);
            }
            return;
        }

        auto itBegin = m_map.upper_bound(keyBegin);
        bool isDeleteItBegin = false;

        // Check if keyBegin is equal to std::prev(itBegin)->first,
        // as we know that the latter is <= to keyBegin
        if (itBegin != m_map.begin() && !(std::prev(itBegin)->first < keyBegin))
        {
            itBegin--;
        }

        if (itBegin == m_map.begin()) {
            auto res = m_map.insert_or_assign(keyBegin, val);
            itBegin = res.first;
            isDeleteItBegin = isValEqValBegin;
        }
        else if (itBegin == m_map.end()) {

            if ((std::prev(itBegin))->second == val) {
                // itBegin is the largest element, and its value is already correct.
                return;
            }

            V valTrailing = (std::prev(itBegin))->second;
            m_map[keyBegin] = val;
            m_map[keyEnd] = std::move(valTrailing);
            return;
        }
        else {  // itBegin is somewhere in the middle of m_map

            if ((std::prev(itBegin))->second == val) {
                itBegin--;
            }
            else {
                auto res = m_map.insert_or_assign(keyBegin, val);
                itBegin = res.first;
            }
        }

        // Remove all entries with key K in m_map where keyBegin < K < upperbound(keyEnd).
        for (auto it = (isDeleteItBegin? itBegin : std::next(itBegin)); it != itEnd;)
        {
            it = m_map.erase(it);
        }

        if (val != valEnd)
        {
            m_map.insert_or_assign(keyEnd, std::move(valEnd));
        }
    }

    // look-up of the value associated with key
    V const& operator[]( K const& key ) const {
        auto it=m_map.upper_bound(key);
        if(it==m_map.begin()) {
            return m_valBegin;
        } else {
            return (--it)->second;
        }
    }

    void print() const {
        for (const auto &node : m_map) {
            std::cout << "\t[ " << node.first << " ] => \'" << node.second << "\'" << std::endl;
        }
    }

    std::size_t GetSize() const {
        return m_map.size();
    }

    bool isCanonical() const {
        V lastVal = m_valBegin;
        for (auto it = m_map.begin(); it != m_map.end(); it++)
        {
            if (it->second == lastVal)
                return false;

            lastVal = it->second;
        }
        return true;
    }
};

// Many solutions we receive are incorrect. Consider using a randomized test
// to discover the cases that your implementation does not handle correctly.
// We recommend to implement a test function that tests the functionality of
// the interval_map, for example using a map of int intervals to char.

bool RunTest(int keyBegin, int keyEnd, char val)
{
    char outputExpected[9];

    interval_map<int, char> test_map('A');
    for (int i = 0; i < 9; i++)
    {
        outputExpected[i] = 'A';
    }

    test_map.assign(3, 5, 'B');
    for (int i = 3; i < 5; i++)
    {
        outputExpected[i] = 'B';
    }

    test_map.assign(keyBegin, keyEnd, val);
    for (int i = keyBegin; i < keyEnd; i++)
    {
        outputExpected[i] = val;
    };

    bool isFailed = false;
    for (int i = 0; i < 9; i++)
    {
        if (outputExpected[i] != test_map[i])
        {
            isFailed = true;
            break;
        }
    }

    if (isFailed)
    {
        std::cout << "RunTest(" << keyBegin << "," << keyEnd << ",\'" << val << "\') found mismatched output:" << std::endl;
        for (int i = 0; i < 9; i++)
        {
            std::cout << i;
        }
        std::cout << std::endl;
        for (int i = 0; i < 9; i++)
        {
            std::cout << outputExpected[i];
        }
        std::cout << std::endl;
        for (int i = 0; i < 9; i++)
        {
            std::cout << test_map[i];
        }
        std::cout << std::endl;

        test_map.print();

        std::cout << std::endl;
    }

    if (!test_map.isCanonical())
    {
        std::cout << "RunTest(" << keyBegin << "," << keyEnd << ",\'" << val << "\') is non-canonical:" << std::endl;
        test_map.print();
    }

    return test_map.isCanonical();
}

void BruteForceTest()
{
    std::vector<std::string> vecNotCanonical;

    for (int i = 1; i <= 7; i++)
    {
        for (int j = 1; j <= 7; j++)
        {
            for (char c = 'A'; c <= 'C'; c++)
            {
                if (!RunTest(i, j, c))
                {
                    vecNotCanonical.push_back("RunTest(" + std::to_string(i) + "," + std::to_string(j) + ",\'" + c + "\') is not canonical");
                }
            }
        }
    }

    if (vecNotCanonical.empty())
    {
        std::cout << "All canonical!" << std::endl;
    }
}

bool RunTest2(int keyBegin, int keyEnd, char val)
{
    char outputExpected[13];

    interval_map<int, char> test_map('A');
    for (int i = 0; i < 13; i++)
    {
        outputExpected[i] = 'A';
    }

    test_map.assign(3, 5, 'B');
    for (int i = 3; i < 5; i++)
    {
        outputExpected[i] = 'B';
    }

    test_map.assign(5, 7, 'C');
    for (int i = 5; i < 7; i++)
    {
        outputExpected[i] = 'C';
    }

    test_map.assign(7, 9, 'B');
    for (int i = 7; i < 9; i++)
    {
        outputExpected[i] = 'B';
    }

    test_map.assign(keyBegin, keyEnd, val);
    for (int i = keyBegin; i < keyEnd; i++)
    {
        outputExpected[i] = val;
    };

    bool isFailed = false;
    for (int i = 0; i < 13; i++)
    {
        if (outputExpected[i] != test_map[i])
        {
            isFailed = true;
            break;
        }
    }

    if (isFailed)
    {
        std::cout << "RunTest(" << keyBegin << "," << keyEnd << ",\'" << val << "\') found mismatched output:" << std::endl;
        for (int i = 0; i < 13; i++)
        {
            std::cout << i;
        }
        std::cout << std::endl;
        for (int i = 0; i < 13; i++)
        {
            std::cout << outputExpected[i];
        }
        std::cout << std::endl;
        for (int i = 0; i < 13; i++)
        {
            std::cout << test_map[i];
        }
        std::cout << std::endl;

        test_map.print();

        std::cout << std::endl;
    }

    if (!test_map.isCanonical())
    {
        std::cout << "RunTest(" << keyBegin << "," << keyEnd << ",\'" << val << "\') is non-canonical:" << std::endl;
        test_map.print();
    }

    return test_map.isCanonical();
}

void BruteForceTest2()
{
    std::vector<std::string> vecNotCanonical;

    for (int i = 1; i <= 13; i++)
    {
        for (int j = 1; j <= 13; j++)
        {
            for (char c = 'A'; c <= 'D'; c++)
            {
                if (!RunTest2(i, j, c))
                {
                    vecNotCanonical.push_back("RunTest2(" + std::to_string(i) + "," + std::to_string(j) + ",\'" + c + "\') is not canonical");
                }
            }
        }
    }

    if (vecNotCanonical.empty())
    {
        std::cout << "All canonical 2!" << std::endl;
    }
}


int main()
{
    //BruteForceTest();
    BruteForceTest2();
    //RunTest2(11,11,'A');
}