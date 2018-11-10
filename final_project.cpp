/* John McCarthy
6/7/2018 CIT-237-01
Final Project : Genetic Algorithm to evolve a population of
randome cstrings to match a target phrase */
#include <string> // include string
#include <cctype> // include cc type for character manipulation
#include <iostream> // for getline
#include <ctime> // ctime for rand seeed
#include <vector> // include vector
using namespace std; // define std namespace

					 // constant lookup table for alphanumeric characters
const char alphanum[] =
"0123456789 "
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";

static char generateRandomChar() {
	// select a random character from the alphanum constant and return it
	return alphanum[rand() % ((sizeof(alphanum) / sizeof(*alphanum)) - 1)];
}


class Dna {

	int fitness;
	double selectionProbability;
public:
	vector<char> genes;
	Dna();
	Dna(int);
	Dna(const Dna &);
	vector<char> getPhrase();
	void calcFitness(string);
	Dna crossover(Dna, double);
	void mutate(double);
	double getFitness() { return fitness; }
	void setSelectionProbability(double sProb) { selectionProbability = sProb; }
	double getSelectionProbability() { return selectionProbability; }
	void addGene(char);
};

class Population {
	// Holds an array of the current DNA objects in the population
	vector<Dna> population;
	// Vector to hold mating pool for selecting parents
	vector<Dna> matingPool;
	// Counter to keep track of the # of generations that have been created
	int generations = 0;
	// Boolean to keep track of if target phrase was found
	bool targetFound = false;
	// cstring holding the target phrase
	string target;
	// double holding the mutation rate for the population
	double mutationRate;
	// int holding the "perfect" fitness score
	double perfectScore = 1.0;
	// string that holds the current best member of a generation
	string best = "";
public:
	Population(string, double, int);
	~Population() {};
	void generate();
	void findBest();
	bool isTargetFound();
	string getBest() { return best; }
	void calculateFitness();
	Dna selectPopulationMember();
	void generateMatingPool();
	int getGeneration() { return generations; }
};



// Constrcutor
Population::Population(string targetString,
	double mRate,
	int populationSize) {
	// Set the perfect score based on size of targetString
	// this->perfectScore = targetString.length();
	// Set the target using the passed string
	this->target = targetString;
	this->perfectScore = targetString.length() * targetString.length();
	// Set the mutation rate
	this->mutationRate = mRate;
	// Add appropriate number of DNA objects based on the population size passed
	for (int i = 0; i < (populationSize); i++) {
		this->population.push_back(Dna(targetString.length()));
	}
	// Calculate the fitness of each of the members of the population
	calculateFitness();
}

void Population::calculateFitness() {
	// Go through each DNA object and call its individudal calc fitness method
	// keep track of found fitnesses in sum counter for assigning
	// each items selectionProbability
	int sum = 0;
	for (int i = 0; i < population.size(); i++) {
		population[i].calcFitness(target);
		sum += population[i].getFitness();
	}

	for (int i = 0; i < population.size(); i++) {
		// population[i].setSelectionProbability((double)population[i].getFitness() / (double)sum);
		population[i].setSelectionProbability(population[i].getFitness());
	}
}

// Method to create a new generation of the population
void Population::generate() {
	for (int i = 0; i < population.size(); i++) {
		Dna parentOne = selectPopulationMember();
		Dna parentTwo = selectPopulationMember();
		Dna child = parentOne.crossover(parentTwo, this->mutationRate);
		population[i] = child;
	}
	// incrament the number of generations
	this->generations++;
}

void Population::generateMatingPool() {
	this->matingPool.clear();
	for (int i = 0; i < population.size(); i++) {
		for (int j = 0; j < population[i].getFitness(); j++) {
			this->matingPool.push_back(Dna(population[i]));
		}
	}
}

// Method to find the member of the population with the highest fitness
void Population::findBest() {
	int maxFound = 0;
	int foundIndex = 0;

	for (int i = 0; i < population.size(); i++) {
		if (population[i].getFitness() > maxFound) {
			foundIndex = i;
			maxFound = population[i].getFitness();
		}
	}

	vector<char> foundGenes = population[foundIndex].getPhrase();
	string str(foundGenes.begin(), foundGenes.end());
	best = str;

	if (maxFound == perfectScore) {
		targetFound = true;
	}
}

bool Population::isTargetFound() {
	return targetFound;
}


Dna Population::selectPopulationMember() {
	// int index = 0;
	// double randomNumber = ((double)rand() / (RAND_MAX));
	// while (randomNumber > 0) {
	// 	if (population[index].getSelectionProbability() >= 0) {
	// 		randomNumber = randomNumber - (population[index].getSelectionProbability());
	// 		index++;
	// 	}
	// }
	// index--;
	int randomNumber = (rand() % this->matingPool.size());
	return matingPool[randomNumber];
}

Dna::Dna() {

}

// DNA constructor
Dna::Dna(int targetLength) {
	// set initial fitness to zero
	this->fitness = 0;
	// generate random char array at length of target phrase
	for (int i = 0; i < (targetLength); i++) {
		genes.push_back(generateRandomChar());
	}
}

Dna::Dna(const Dna &dna) {
	this->fitness = dna.fitness;
	this->genes = dna.genes;
}

void Dna::addGene(char geneMember) {
	this->genes.push_back(geneMember);
}

vector<char> Dna::getPhrase() {
	return this->genes;
}


// Method to calculate the fitness of a DNA object in comparison to the target
// phrase
void Dna::calcFitness(string targetPhrase) {
	int score = 0;
	for (int i = 0; i < genes.size(); i++) {
		if (genes[i] == targetPhrase.at(i)) {
			score++;
		}
	}
	//
	this->fitness = score * score;
	// this -> selectionProbability =
}

// Method to generate a child out of 2 DNA objects
Dna Dna::crossover(Dna partner, double mutationRate) {
	// Create a new DNA object
	Dna child = Dna();
	// select a random midpoint to split the parents genes
	int midpoint = rand() % (genes.size() - 1);

	// Assign genes to child from one of the parents based on the midpoint
	for (int i = 0; i < genes.size(); i++) {
		if (i > midpoint) {
			child.addGene(genes[i]);
		}
		else {
			child.addGene(partner.getPhrase()[i]);
		}
	}
	child.mutate(mutationRate);
	// return the new child
	return child;
}

void Dna::mutate(double mutationRate) {
	// generate a random number between 0 and 1
	double randomDouble = ((double)rand() / (RAND_MAX));
	for (int i = 0; i < genes.size(); i++) {
		if (randomDouble < mutationRate) {
			genes[i] = generateRandomChar();
		}
	}
}



void main() {
	srand(time(NULL)); // seed rand
	string targetPhrase = "Wubba Lubba";
	double mutationRate = 0.1;
	int populationSize = 200;
	Population poppy = Population(targetPhrase, 0.1, 200);

	do {
		poppy.generateMatingPool();
		poppy.generate();
		poppy.calculateFitness();
		poppy.findBest();
		cout << poppy.getBest() << endl;
	} while (!poppy.isTargetFound());
	cout << "We found that biz" << endl;
	cout << poppy.getBest() << endl;
	cout << poppy.getGeneration() << endl;
	system("pause");
}
