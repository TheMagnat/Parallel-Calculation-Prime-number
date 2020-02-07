
#include "MoinsNaive.hpp"

///Ce constructeur va préremplir le tableau de toute les valeurs par des vraie et va mettre 0 et 1 à faux.


MoinsNaive::MoinsNaive(size_t maxN, size_t nbThread) : N_(maxN), nbThread_(nbThread), sema_(nbThread_) {}


//Ancienne version du process
void MoinsNaive::processFromTo(size_t start, size_t stop, size_t partIndex){

	bool startedImpair(start & 1);

	//On stoquera tout les premiers trouvé dans ce vecteur.
	std::vector<size_t> allFound;

	//On prépare un vateur qui représentera toute les valeur de start à stop privé des pairs.
	std::vector<bool> values((stop - start + startedImpair)/2, true);

	//On avance de 2 en 2 pour eviter les pairs. (et on commence à 3)
	for (size_t i(3); i*i <= stop; i += 2){

		/*
			Optimisation sympa pour quand l'on atteind de grand i.
			On verifie les premier nombre premier.
			(Note : Cette optimisation est un peu comme si l'on fusionait le crible et la méthode naive.
			Cette optimisation n'est pas de moi, je me suis inspiré de quelque recherche faite sur internet.)
		*/
		if(i >= 3*3	&& i % 3 == 0){
			continue;
		}
		if(i >= 5*5 && i % 5 == 0){
			continue;
		}
		if(i >= 7*7 && i % 7 == 0){
			continue;
		}
		if(i >= 11*11 && i % 11 == 0){
			continue;
		}
		if(i >= 13*13 && i % 13 == 0){
			continue;
		}


		//Ce calcul permet de trouver le multiple de i superieur à start le plus petit.
		//On enlève 1 car si start est un multiple de i on le dépasserais.
		size_t value = ((start-1)/i)*i + i;

		/*
		Si la valeur est inférieur au carré,
		les valeur de value à i*i ont déjà été traité avec les i précedent.
		On place donc directement value à i*i.
		*/
		if (value < i*i){
			value = i*i;
		}

		/*
		Si le test précédent n'est pas vérifié,
		il n'est pas dit que le multiple de i superieur à start le plus petit ne soit pas pair.
		Vue que nous avons éliminé les pairs, si value est pair,
		on rajoute i à ce dernier pour revenir au nombre multiple de i impair le plus petit après start.
		*/
		else if ((value & 1) == 0){
			value += i;
		}

		/*
		
		value etant donc un multiple de i, il n'est pas premier.
		On part de ce dernier et on boucle sur tout les multiple de i supérieur
		et on les met sur faux pour dire qu'ils ne sont pas entier.

		*/
		for (size_t j = value; j < stop; j += 2*i){

			size_t index = j - start;
			values[index/2] = false;

		}


	}


	

	for (size_t i = 0; i < values.size(); i++){
		if(values[i] == true) {
			allFound.emplace_back(i*2 + start + !startedImpair);
		}
	}


	allResults_[partIndex] = std::move(allFound);


	sema_.notify();

}

///Modification de dernière minute pour ne pas avoir à créer plus de nbThread_.
void MoinsNaive::processFromToWithStep(size_t step, size_t index){

	//Le gap est la distance entre chaque thread en terme de nombre.
	//Quand on fini pour un certain start et stop, on les avance de gap.
	size_t gap(step * nbThread_);

	for(size_t start(step * index + 2), stop(step * (index + 1) + 2), partIndex(index + 1); start <= N_; start += gap, stop += gap, partIndex += nbThread_){

		if(stop > (N_ + 1)){
			stop = (N_ + 1);
		}

		bool startedImpair(start & 1);

		//On stoquera tout les premiers trouvé dans ce vecteur.
		std::vector<size_t> allFound;

		//On prépare un vateur qui représentera toute les valeur de start à stop privé des pairs.
		std::vector<bool> values((stop - start + startedImpair)/2, true);

		//On avance de 2 en 2 pour eviter les pairs. (et on commence à 3)
		for (size_t i(3); i*i <= stop; i += 2){

			/*
				Optimisation sympa pour quand l'on atteind de grand i.
				On verifie les premier nombre premier.
				(Note : Cette optimisation est un peu comme si l'on fusionait le crible et la méthode naive.
				Cette optimisation n'est pas de moi, je me suis inspiré de quelque recherche faite sur internet.)
			*/
			if(i >= 3*3	&& i % 3 == 0){
				continue;
			}
			if(i >= 5*5 && i % 5 == 0){
				continue;
			}
			if(i >= 7*7 && i % 7 == 0){
				continue;
			}
			if(i >= 11*11 && i % 11 == 0){
				continue;
			}
			if(i >= 13*13 && i % 13 == 0){
				continue;
			}


			//Ce calcul permet de trouver le multiple de i superieur à start le plus petit.
			//On enlève 1 car si start est un multiple de i on le dépasserais.
			size_t value = ((start-1)/i)*i + i;

			/*
			Si la valeur est inférieur au carré,
			les valeur de value à i*i ont déjà été traité avec les i précedent.
			On place donc directement value à i*i.
			*/
			if (value < i*i){
				value = i*i;
			}

			/*
			Si le test précédent n'est pas vérifié,
			il n'est pas dit que le multiple de i superieur à start le plus petit ne soit pas pair.
			Vue que nous avons éliminé les pairs, si value est pair,
			on rajoute i à ce dernier pour revenir au nombre multiple de i impair le plus petit après start.
			*/
			else if ((value & 1) == 0){
				value += i;
			}

			/*
			
			value etant donc un multiple de i, il n'est pas premier.
			On part de ce dernier et on boucle sur tout les multiple de i supérieur
			et on les met sur faux pour dire qu'ils ne sont pas entier.

			*/
			for (size_t j = value; j < stop; j += 2*i){

				size_t index = j - start;
				values[index/2] = false;

			}


		}


		

		for (size_t i = 0; i < values.size(); i++){
			if(values[i] == true) {
				allFound.emplace_back(i*2 + start + !startedImpair);
			}
		}

		//On bouge la data de allFound dans le vecteur de allResults correspondant a la bonne partie
		allResults_[partIndex] = std::move(allFound);


	}

	//BOUCLE CONTINUE

}

/*
	Fonction principale qui lance tout les calculs.
*/
void MoinsNaive::start(){

	/*
		Avec cette méthode, le step choisi influe grandement sur le temps.
		J'ai trouvé qu'un step égale à N max divisé par 1000 donne de bon résultat donc je l'utilise.
		On pourrait passer cette valeur en pramètre pour une potentiel amélioration..
	*/

	size_t step(N_/1000);

	//Si N max est trop petit N/1000 est trop ridicule pour être bon.
	if(N_ < 100000){
		step = 1000;
	}

	/*
		Le nombre de fois que l'on va créer de vecteur pour reserver déjà de la place pour ces dernier.
		Cela permet de ne pas avoir besoins de mutex et de push chaque valeur dans le même vecteur.
		On garde aussi les valeur ordonné de cette manière.
	*/
	size_t iterationNumber(ceil((float)(N_-1)/step));
	allResults_.resize(iterationNumber+1);


	/*
	On ajoute 2 au tout début car il sagit du seul nombre pair et premier.
	Le fait que cela nous force a créer un vecteur d'un seul élément pour presque rien n'est pas dérangeant,
	Cela reste souvent moins couteux que de vérifié si dans la fonction processFromTo si from comprend 2 pour l'ajouter à chaque fois.
	*/
	allResults_[0] = std::vector<size_t>{2};

	
	std::vector<std::thread> allThread_;

	for (size_t i(0); i < nbThread_; ++i){				

		allThread_.emplace_back(&MoinsNaive::processFromToWithStep, this, step, i);

	}

	for (size_t i(0); i < nbThread_; ++i){	

		allThread_[i].join();

	}



}

//Ancienne version du start qui fonctionne avec l'ancienne version de process, processFromTo.
// void MoinsNaive::start(){

// 	/*
// 		Avec cette méthode, le step choisi influe grandement sur le temps.
// 		J'ai trouvé qu'un step égale à N max divisé par 1000 donne de bon résultat donc je l'utilise.
// 		On pourrait passer cette valeur en pramètre pour une potentiel amélioration..
// 	*/

// 	size_t step(N_/1000);

// 	//Si N max est trop petit N/1000 est trop ridicule pour être bon.
// 	if(N_ < 100000){
// 		step = 1000;
// 	}

// 	/*
// 		Le nombre de fois que l'on va créer de vecteur pour reserver déjà de la place pour ces dernier.
// 		Cela permet de ne pas avoir besoins de mutex et de push chaque valeur dans le même vecteur.
// 		On garde aussi les valeur ordonné de cette manière.
// 	*/
// 	size_t iterationNumber(ceil((float)(N_-1)/step));
// 	allResults_.resize(iterationNumber+1);


//	/*	
// 	On ajoute 2 au tout début car il sagit du seul nombre pair et premier.
// 	Le fait que cela nous force a créer un vecteur d'un seul élément pour presque rien n'est pas dérangeant,
// 	Cela reste souvent moins couteux que de vérifié si dans la fonction processFromTo si from comprend 2 pour l'ajouter à chaque fois.
//	*/

// 	allResults_[0] = std::vector<size_t>{2};

// 	/*
// 		Cette boucle for s'occupe de lancer les thread avec les bon intervalle pour la fonction processFromTo.
// 	*/
// 	for (size_t start(2), partIndex(0); start <= N_; start += step, ++partIndex){
		

// 		//Cette semaphore est présente pour bloquer le nombre de thread en cours
// 		sema_.wait();

// 		size_t stop = start + step;
		
// 		if (stop > N_){
// 			stop = N_;
// 		}

// 		std::thread newThread(&MoinsNaive::processFromTo, this, start, stop, partIndex+1);

// 		/*
// 		On détache le thread pour ne pas avoir besoins de le stoquer dans un vecteur ou tout autre structure.
// 		La sémaphore nous assure la fin de tout les thread après la boucle et comme ça pas besoins de join.
// 		*/
// 		newThread.detach();

// 	}

// 	//On attend la fin des dernier threads.
// 	sema_.waitAll();


// }


//Fonction d'affichage classique pour du débug.
void MoinsNaive::affiche(){

	size_t count = 0;
	for(size_t i(0), maxSize(allResults_.size()); i < maxSize; ++i){

		std::vector<size_t>& refToVec(allResults_[i]);

		count += refToVec.size();

		//Commenter le block for suivant pour n'afficher que le nombre total de nombre premier.
		for(size_t j(0), maxSize2(refToVec.size()); j < maxSize2; ++j){
			std::cout << refToVec[j] << std::endl;
		}


	}

	std::cout << "Total : " << count << std::endl;

}

/*
	Fonction seulement appelé pour créer un fichier pour les threads.
*/
void MoinsNaive::generateFileFromTo(size_t start, size_t stop, size_t index){

	std::ofstream file;
	
	std::string buffer;

	//Optimisation simple mais intéressante
	size_t len((log10(N_) + 1 /*On ne retire pas 1 ici car on compte le retour a la ligne*/)*(stop - start));

	buffer.reserve(len);

	for(size_t i(start); i < stop; ++i){

		buffer += std::to_string(containAll_[i]);
		buffer.push_back('\n');

	}

	file.open("Resultats/Resultat_17806231_" + std::to_string(N_) + "_(" + std::to_string(index) + ").txt");

	file.write(buffer.data(), buffer.size());

	file.close();

	sema_.notify();

}

/*
	
	Fonction qui s'occupe de la générations de tous les fichier.
	Elle est utilise aussi des thread car l'ecriture de fichier peut être très longue.
	Note : Le programme à été pensé pour deplacer les vecteur, les vecteur de allResults seront donc NULL près cette fonction. il est donc conseillé d'arreter d'utiliser l'objet MoinsNaive après celle-ci.
*/
void MoinsNaive::generateFile(){


	//On fusionne tout les vecteur entre eux.
	for(size_t vectorIndex(0), maxVectorIndex(allResults_.size()); vectorIndex < maxVectorIndex; ++vectorIndex){

		std::vector<size_t>& refToVec(allResults_[vectorIndex]);

		std::move(refToVec.begin(), refToVec.end(), std::back_inserter(containAll_));

	}


	size_t step(MAX_LINE_IN_FILE);

	//On utilise exactement la même boucle que start (lancienne version) ici pour threader aussi l'ecriture de fichier.
	for (size_t start(0), partIndex(1), maxSize(containAll_.size()); start <= maxSize; start += step, ++partIndex){
		
		sema_.wait();

		size_t stop = start + step;
		
		if (stop > maxSize){
			stop = maxSize;
		}

		std::thread newThread(&MoinsNaive::generateFileFromTo, this, start, stop, partIndex);

		newThread.detach();

	}

	sema_.waitAll();


}