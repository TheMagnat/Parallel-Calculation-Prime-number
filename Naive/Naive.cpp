
#include "Naive.hpp"

///Constructeur très basique
Naive::Naive(size_t maxN, size_t nbThread) : N_(maxN), nbThread_(nbThread) {



}


/*
Cette méthode s'occupe de tout les calcul. Elle calcul pour chaque nombre si il à un diviseur.
Optimisation :
    On ignore naturellement tout les multiples de deux en avançant naturellement de 2 * le pas.
    On commence à l'index du thread et on avance du nombre de thread (Chaque valeur est * 2 pour ignorer les deux)
    Quand on test les possible multiple d'un nombre on s'arrete à la racine carré de celui-ci car un multiple d'un nombre ne peux pas dépasser cette valeur.
*/
void Naive::calculateWithJumb(size_t index){



    for(size_t i((index*2) + 3); i < N_; i += (nbThread_*2)){

        unsigned int maxValue(sqrt(i));
        bool isFirst = true;
        for(size_t j(3); j <= maxValue; ++j){

            if(i%j == 0){
                isFirst = false;
                break;
            }

        }

        if(isFirst){
            std::lock_guard<std::mutex> lock(lockFirst_);
            allFirst_.emplace_back(i);
        }

    }

}


/*
Cette méthode va s'occuper de créer nbThread_ Thread sur calculateWithJump avec leur index respectif.
Une légère optimisation est faite ici en reservant déjà une taille de N_ / 10 dans le tableau de nombre premier car on estime qu'il y en aura un peu de moins de cette valeur. Cela permet de n'avoir qu'une seule grosse allocation mémoire au début du programme plutot que réallouer a chaque fois que l'on dépasse la taille reservé.
On rajoute aussi 2 dans celui-ci vu qu'il est totalement ignoré dans calculateWithJump.
*/
void Naive::start(){

    allFirst_.reserve(N_/10);
    
    allFirst_.emplace_back(2);


    for(size_t i(0); i < nbThread_; ++i){

        allThread_.emplace_back(&Naive::calculateWithJumb, this, i);

    }

    for(size_t i(0); i < nbThread_; ++i){

        allThread_[i].join();

    }


}

///Fonction de debugage pour afficher les premiers trouvé.
void Naive::affiche(){
    for(size_t i(0), maxSize(allFirst_.size()); i < maxSize; ++i){
        std::cout << allFirst_[i] << std::endl;
    }

    std::cout << "Total : " << allFirst_.size() << std::endl;
}

        