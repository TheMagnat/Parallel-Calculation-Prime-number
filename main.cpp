
#include "Naive/Naive.hpp"

#include "MoinsNaive/MoinsNaive.hpp"

#include <chrono>

/*

Le code est commenté dans chaque fichier.
Note : Il y à peut être des endroit mal commenté, j'ai fini vite par manque de temps.

Compiler avec make,
L'executable se nomme "a.out",
Il écrira tout les fichier de sortie dans le dossier Resultats normalement créé aussi par le makefile.
Les résultats sont coupé en plusieur fichier, chaque fichier à une taille max de 3 millions de lignes.

*/



/*

main très basique qui va lire les argument passé à l'executable et appeller ma classe avec ces dernier.

*/


int main(int argc, char const *argv[]){

    if(argc < 3){
        std::cout << "Invalid argument, usage : " << argv[0] << " [Max Value] [Number of Thread]" << std::endl;
        return -1;
    }


    //Très important, utiliser stol a la place de atoi pour convertir en long plutot qu'en int Car sinon la limite est de 1milliard4 environ.
    size_t maxN = std::stol(argv[1]);
    size_t nbThread = std::stol(argv[2]);

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    /*
	
	J'ai aussi implémenté la méthode naive.
	Je n'ai pas créer de moyen de la lancer sans simplement decommenter
	et je n'ai pas implémenté de méthode pour générer des fichier pour la simple raison
	que la seconde méthode est beaucoup beaucoup plus efficace.

    */

    // Naive myNaive(maxN, nbThread);

    // myNaive.start();

    // myNaive.affiche();
    	
    MoinsNaive myMoinsNaive(maxN, nbThread);

    myMoinsNaive.start();

    std::cout << std::endl << "1 CALCULS - Temps de calculs pour trouver les nombres premier jusqu'a " << maxN << " = " << std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::steady_clock::now() - begin).count()/1000.0f << " seconds" << std::endl << std::endl;

    begin = std::chrono::steady_clock::now();

    //Pour afficher dans le terminal décommenter ici.
    //myMoinsNaive.affiche();

    std::cout << "Créations des fichiers..." << std::endl << std::endl;

    //Commenter cette ligne pour désactiver la création de fichier et juste recuperer le temps mis pour les calculs.
    myMoinsNaive.generateFile();

    std::cout << "2 FILES - Temps pour la créations des fichier = " << std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::steady_clock::now() - begin).count()/1000.0f << " seconds" << std::endl << std::endl;

    return 0;

}