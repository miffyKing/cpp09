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

bool PmergeMe::_isEqual(const std::pair<int, int> &e, int val)
{
    return e.first == val;
}

size_t PmergeMe::_getIndex(int k)
{
    if (k == 1 || k == 0)
        return 1;
    return (std::pow(2, k + 1) + std::pow(-1, k)) / 3;
}

void PmergeMe::_seperate_vector(std::vector<int> &vec, std::vector<int> &large, std::vector<int> &small)
{
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

std::vector<int> PmergeMe::fordjohnson_vector(std::vector<int> &vec)
{
    if (vec.size() == 1)
        return vec;
    std::vector<int> ret;
    std::vector<int> large;
    std::vector<int> small;

    std::vector<std::pair<int, int> > pairs;

    _seperate_vector(vec, large, small);

    for (size_t i = 0; i < large.size(); i++)
        pairs.push_back(std::make_pair(large[i], small[i]));
    if (large.size() < small.size())
        pairs.push_back(std::make_pair(0, small[small.size() - 1]));

    large = fordjohnson_vector(large);

    // std::cout<<" pairs size is " << pairs.size() << "\n";

    for (size_t i = 0; i < pairs.size(); i++)
    {
        std::pair<int, int> &p = pairs[i];
        for (size_t j = 0; j < large.size(); j++)
            if (_isEqual(p, large[j]))
                small[j] = p.second;
    }

    ////////////////////

    ret.push_back(small[0]);

    // std::cout<<"LARGE : ";
    // for (int i = 0; i < (int)large.size(); i++)
    // {
    //     std::cout<< large[i] << " ";
    // } std::cout<<"\n";
    // std::cout<< "SMALL : ";
    // for (int i = 0; i < (int)small.size(); i++)
    // {
    //     std::cout<< small[i] << " ";
    // } std::cout<<"\n";

    for (size_t i = 0; i < large.size(); i++)
        ret.push_back(large[i]);

    int k = 2;

    while (_getIndex(k - 1) <= large.size())
    {
        int m = std::min(_getIndex(k) - 1, small.size() - 1);
        for (size_t i = m; i > _getIndex(k - 1) - 1; i--)
        {
            _binaryInsert(ret, small[i]);
        }
        k += 1;
    }
    return ret;
}

//--------------------------vector--------------------------------

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

std::deque<int> PmergeMe::fordjohnson_deque(std::deque<int> &deq)
{
    if (deq.size() == 1)
        return deq;
    std::deque<int> ret;
    std::deque<int> large;
    std::deque<int> small;

    std::deque<std::pair<int, int> > pairs;
    _seperate_deque(deq, large, small);
    for (size_t i = 0; i < large.size(); i++)
        pairs.push_back(std::make_pair(large[i], small[i]));
    if (large.size() < small.size())
        pairs.push_back(std::make_pair(0, small[small.size() - 1]));

    large = fordjohnson_deque(large);

    for (size_t i = 0; i < pairs.size(); i++)
    {
        std::pair<int, int> &p = pairs[i];
        for (size_t j = 0; j < large.size(); j++)
            if (_isEqual(p, large[j]))
                small[j] = p.second;
    }
    ret.push_back(small[0]);

    for (size_t i = 0; i < large.size(); i++)
        ret.push_back(large[i]);
    int k = 2;
    while (_getIndex(k - 1) <= large.size())
    {
        int m = std::min(_getIndex(k) - 1, small.size() - 1);
        for (size_t i = m; i > _getIndex(k - 1) - 1; i--)
        {
            _binaryInsert(ret, small[i]);
        }
        k += 1;
    }
    return ret;
}

//--------------------------deque--------------------------------