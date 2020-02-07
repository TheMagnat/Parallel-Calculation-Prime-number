
#include "Semaphore.hpp"

#include <vector>

#include <thread>
#include <mutex>

#include <iostream>

#include <cmath>

#include <fstream>

//Les fichiers feront MAX_LINE_IN_FILE lignes.
#define MAX_LINE_IN_FILE 3000000

/*

Cette classe contient la méthode du Crible d'Ératosthène.
Il sagit d'une version un peu modifier pour être lancable d'un index de deppart à un index de fin,
très pratique pour la parallèlisé.
L'idée de cinder cet algorithme n'est pas une idée de moi mais est déjà bien connue et est expliqué en ligne.

*/
class MoinsNaive{

	public:

		MoinsNaive(size_t maxN, size_t nbThread);


		void processFromTo(size_t start, size_t stop, size_t partIndex);

		void processFromToWithStep(size_t step, size_t index);

		/*
			Fonction principale qui lance tout les calculs.
		*/
		void start();


		//Fonction d'affichage classique pour du débug.
		void affiche();

		/*
			Fonction seulement appelé pour créer un fichier pour les threads.
		*/
		void generateFileFromTo(size_t start, size_t stop, size_t index);

		/*
			
			Fonction qui s'occupe de la générations de tous les fichier.
			Elle est utilise aussi des thread car l'ecriture de fichier peut être très longue.

		*/
		void generateFile();

	private:

		//Va stoquer les resultat de chaque partie.
		std::vector<std::vector<size_t>> allResults_;

		//Va stoquer le resultat des vecteurs de allResults_ concaténé
		std::vector<size_t> containAll_;

		//Notre N max
		size_t N_;
		//Le nombre de threads
		size_t nbThread_;


		//Une semaphore bien pratique pour les threads.
		Semaphore sema_;

};