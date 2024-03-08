#include "PmergeMe.hpp"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Error : Not enough argument\n";
        return 1;
    }
    PmergeMe pm;

    for (int i = 1; i < argc; i++)
    {
        int value = std::atoi(argv[i]);
        if (value <= 0)
        {
            std::cerr << "Error : value negative" << std::endl;
            return 1;
        }
        for (size_t idx = 0; idx < std::strlen(argv[i]); idx++)
        {
            if (!std::isdigit(argv[i][idx]))
            {
                std::cerr << "Error : Not a digit " << std::endl;
                return 1;
            }
        }
        pm.fillContainer(value);
    }

    std::cout << "Before: ";
    for (size_t i = 0; i < pm._vector.size(); i++)
        std::cout << pm._vector[i] << ' ';
    std::cout << std::endl;

    std::clock_t v_start = std::clock();
    pm._vector = pm._fordjohnson_vector(pm._vector);
    std::clock_t v_end = std::clock();

    std::clock_t d_start = std::clock();
    pm._deque = pm._fordjohnson_deque(pm._deque);
    std::clock_t d_end = std::clock();

    std::cout << "After(vector): ";
    for (size_t i = 0; i < pm._vector.size(); i++)
        std::cout << pm._vector[i] << ' ';
    std::cout << std::endl;
    std::cout << "After(deque): ";
    for (size_t i = 0; i < pm._deque.size(); i++)
        std::cout << pm._deque[i] << ' ';
    std::cout << std::endl;
    std::cout << "Time to process a range of " << pm._vector.size() << " elements with std::vector : " << (v_end - v_start) << " us" << std::endl;
    std::cout << "Time to process a range of " << pm._deque.size() << " elements with std::deque : " << (d_end - d_start) << " us" << std::endl;
}
