#include "PmergeMe.hpp"

PmergeMe::PmergeMe() {}

PmergeMe::~PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe &obj)
    : _vector(obj._vector), _deque(obj._deque) {}

PmergeMe &PmergeMe::operator=(const PmergeMe &obj)
{
    if (this != &obj)
    {
        _vector = obj._vector;
        _deque = obj._deque;
    }
    return *this;
}

void PmergeMe::fillContainer(int n)
{
    _vector.push_back(n);
    _deque.push_back(n);
}

size_t PmergeMe::_get_jacobsthal_index(int k)
{
    if (k == 1 || k == 0)
        return 1;
    int sorted = std::pow(2, k + 1) + std::pow(-1, k) / 3;
    return sorted;
}

void PmergeMe::_generate_small_large(std::vector<int> &vec, std::vector<int> &large, std::vector<int> &small)
{
    size_t half_size = vec.size() / 2;
    small.reserve(half_size + 1);
    large.reserve(half_size);

    for (size_t i = 0; i < vec.size() - 1; i += 2)
    {
        if (vec[i] < vec[i + 1])
        {
            small.push_back(vec[i]);
            large.push_back(vec[i + 1]);
        }
        else
        {
            small.push_back(vec[i + 1]);
            large.push_back(vec[i]);
        }
    }
    if (vec.size() % 2 != 0)
        small.push_back(vec[vec.size() - 1]);
}

std::vector<int> PmergeMe::_fordjohnson_vector(std::vector<int> &vec)
{
    if (vec.size() == 1)
        return vec;
    std::vector<int> sorted;
    std::vector<int> large;
    std::vector<int> small;

    std::vector<std::pair<int, int> > pairs;

    _generate_small_large(vec, large, small);

    for (size_t i = 0; i < large.size(); i++)
        pairs.push_back(std::make_pair(large[i], small[i]));
    if (large.size() < small.size())
        pairs.push_back(std::make_pair(0, small[small.size() - 1]));

    large = _fordjohnson_vector(large);

    for (size_t i = 0; i < pairs.size(); i++)
    {
        std::pair<int, int> &p = pairs[i];
        for (size_t j = 0; j < large.size(); j++)
            if (p.first == large[j])
                small[j] = p.second;
    }

    sorted.push_back(small[0]);

    sorted.insert(sorted.end(), large.begin(), large.end());

    int k = 2;

    while (_get_jacobsthal_index(k - 1) <= large.size())
    {
        int m = std::min(_get_jacobsthal_index(k) - 1, small.size() - 1);
        for (size_t i = m; i > _get_jacobsthal_index(k - 1) - 1; i--)
        {
            _binaryInsert(sorted, small[i]);
        }
        k += 1;
    }
    return sorted;
}

void PmergeMe::_seperate_deque(std::deque<int> &deq, std::deque<int> &large, std::deque<int> &small)
{
    for (size_t i = 0; i < deq.size() - 1; i += 2)
    {
        if (deq[i] < deq[i + 1])
        {
            small.push_back(deq[i]);
            large.push_back(deq[i + 1]);
        }
        else
        {
            small.push_back(deq[i + 1]);
            large.push_back(deq[i]);
        }
    }
    if (deq.size() % 2 != 0)
        small.push_back(deq[deq.size() - 1]);
}

std::deque<int> PmergeMe::_fordjohnson_deque(std::deque<int> &deq)
{
    if (deq.size() == 1)
        return deq;
    std::deque<int> sorted;
    std::deque<int> large;
    std::deque<int> small;

    std::deque<std::pair<int, int> > pairs;
    _seperate_deque(deq, large, small);
    for (size_t i = 0; i < large.size(); i++)
        pairs.push_back(std::make_pair(large[i], small[i]));
    if (large.size() < small.size())
        pairs.push_back(std::make_pair(0, small[small.size() - 1]));

    large = _fordjohnson_deque(large);

    for (size_t i = 0; i < pairs.size(); i++)
    {
        std::pair<int, int> &p = pairs[i];
        for (size_t j = 0; j < large.size(); j++)
            if (p.first == large[j])
                small[j] = p.second;
    }
    sorted.push_back(small[0]);

    sorted.insert(sorted.end(), large.begin(), large.end());

    int k = 2;
    while (_get_jacobsthal_index(k - 1) <= large.size())
    {
        int m = std::min(_get_jacobsthal_index(k) - 1, small.size() - 1);
        for (size_t i = m; i > _get_jacobsthal_index(k - 1) - 1; i--)
        {
            _binaryInsert(sorted, small[i]);
        }
        k += 1;
    }
    return sorted;
}