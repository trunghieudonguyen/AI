#include "option.h"
#include "hypergraph.hpp"
#include <iostream>
#include <ctime>
#include <cmath>

using namespace std;

int main(int argc, char ** argv)
{
	srand(time(NULL));
	
	OptionParser op(argc, argv);
	if (!op.validCheck()){
		printf("Parameters error, please check the readme.txt file for correct format!\n");
		return -1;
	}
	char * inFile = op.getPara("-i");
	if (inFile == NULL){
		inFile = (char*)"network.bin";
	}

	char * benFile = op.getPara("-b");
        if (inFile == NULL){
                inFile = (char*)"nodeBenefit.txt";
        }

	char * costFile = op.getPara("-c");
        if (inFile == NULL){
                inFile = (char*)"nodeCost.txt";
        }

        char * model = op.getPara("-m");
        if (model == NULL)
                model = (char *) "LT";

	Graph g;
	if (strcmp(model, "LT") == 0){
		g.readGraphLT(inFile,benFile);
	} else if (strcmp(model, "IC") == 0){
		g.readGraphIC(inFile,benFile);
	} else {
		printf("Incorrect model option!");
		return -1;
	}

	int n = g.getSize();

	vector<float> cost(n+1,0);
	ifstream cFile(costFile);
	float tmpF = 0;
	for (int i = 0; i < n; ++i){
		cFile >> tmpF;
		if (tmpF <= 1e-9)
			cost[i] = 1e-9;
		else
			cost[i] = tmpF;
	}
	cFile.close();

	char * tmp = op.getPara("-epsilon");
	float epsilon = 0.1;
	if (tmp != NULL){
		epsilon = atof(tmp);
	}

	float delta = 1.0/n;
	double k = 100;
	
	tmp = op.getPara("-delta");
	if (tmp != NULL){
		delta = atof(tmp);
	}

	tmp = op.getPara("-k");
	if (tmp != NULL){
		k = atof(tmp);
	}

	int t = 1;
	tmp = op.getPara("-t");
	if (tmp != NULL){
		t = atoi(tmp);
	}

	double iter=k;
	
	HyperGraph hg(n,g);
	vector<double> degree(n+1,0);
	double lognk = lgamma(n+1) - lgamma(k+1) - lgamma(n-k-1);
	
	double epsilon2 = sqrt((1-1/exp(1))*log(lognk*2/delta))/((1-1/exp(1))*sqrt(log(4/delta))+sqrt((1-1/exp(1))*log(lognk*2/delta)))*epsilon;

        double requiredDegree = (2-2/exp(1))*(1+epsilon2)*log(4*lognk/delta)/(epsilon2*epsilon2);

	vector<int> seeds;
	char * outFile = op.getPara("-o");
	if (outFile == NULL){
		outFile = (char *)"network.seeds";
	}
	
	long long int curSamples = (long long) requiredDegree;
	long long int totalDegree = 0;
	int mo = 0;
        if (strcmp(model, "IC") == 0){
                mo = 1;
        }
	
	clock_t start = clock();
	while (totalDegree < requiredDegree){
		seeds.clear();
		addHyperedge(g,hg,t,curSamples,mo);
		curSamples *= 2;
		buildSeedSetCost(hg,seeds,n,iter,cost,degree,totalDegree);
	}
	cout << "Seed Nodes: ";
	ofstream out(outFile);
	for (unsigned int i = 0; i < seeds.size(); ++i){
		cout << seeds[i] << " ";
		out << seeds[i] << endl;
	}
	out.close();
	cout << endl << endl;
	printf("Total Benefit: %0.2lf\n",totalDegree*g.getTotalBenefit()/(double)hg.getNumEdge());
	cout << "Time: " << (float)(clock()-start)/CLOCKS_PER_SEC << "s" << endl;
	cout << "Memory: " << getCurrentMemoryUsage() << " MB" << endl;

}
