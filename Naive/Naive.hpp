
#include <vector>
#include <cmath>

#include <iostream>

#include <thread>
#include <mutex>


class Naive{


    public:
        Naive(size_t maxN, size_t nbThread);

        void calculateWithJumb(size_t index);

        void start();

        void affiche();

    private:

        std::mutex lockFirst_;

        std::vector<size_t> allFirst_;
        std::vector<std::thread> allThread_;

        size_t N_;

        size_t nbThread_;

        

};