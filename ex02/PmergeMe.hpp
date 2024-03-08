#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <deque>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>
#include <algorithm>

class PmergeMe
{

public:
    std::vector<int> _vector;
    std::deque<int> _deque;

    PmergeMe();
    ~PmergeMe();

    void fillContainer(int n);
    std::vector<int> _fordjohnson_vector(std::vector<int> &vec);
    std::deque<int> _fordjohnson_deque(std::deque<int> &lst);

private:
    PmergeMe(const PmergeMe &obj);
    PmergeMe &operator=(const PmergeMe &obj);

    template <typename Container, typename T>
    void _binaryInsert(Container &c, const T &value)
    {
        typename Container::iterator it = std::lower_bound(c.begin(), c.end(), value);
        c.insert(it, value);
    }

    void _generate_small_large(std::vector<int> &vec, std::vector<int> &large, std::vector<int> &small);
    void _seperate_deque(std::deque<int> &deq, std::deque<int> &large, std::deque<int> &small);
    size_t _get_jacobsthal_index(int k);
};

#endif