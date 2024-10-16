#ifndef _HYPERGRAPH_H_
#define _HYPERGRAPH_H_

#include "rwgraph.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <vector>
#include <fstream>
#include <cstring>
#include <random>
#include "mappedHeap.hpp"
#include "HeapData.hpp"

#if defined(_OPENMP)
#include <omp.h>
#else
typedef int omp_int_t;
inline omp_int_t omp_set_num_threads(int t) { return 1;}
inline omp_int_t omp_get_thread_num() { return 0;}
#endif

using namespace std;

/*
* building the hypergraph procedure which generates hyperedges following LT model
*/
long long addHyperedge(Graph & g, HyperGraph & hg, int t, long long num, bool lt)
{
	int numNodes = g.getSize();

	omp_set_num_threads(t);
	
	long long iter = 0;
	int c = 10000;

        #pragma omp parallel
	{
		vector<int> visit_mark(numNodes+1,0);
		vector<bool> visit(numNodes+1,false);
		vector<unsigned int> link;
		if (lt == 0){
			while (iter < num){
       	         	       for (int i = 0; i < c; ++i){
	                       	        hg.pollingLT1(g,visit,visit_mark);
	                       	}		
				#pragma omp atomic
				iter += c;
			}
		} else {
			while (iter < num){
        	               for (int i = 0; i < c; ++i){
                        	        hg.pollingIC1(g,visit,visit_mark);
	                        }

        	                #pragma omp atomic
                	        iter += c;
			}
		}

	}
	hg.updateDeg();
	return hg.getNumEdge();
}

/*
* find seed nodes procedure using greedy algorithm
*/
void buildSeedSetCost(HyperGraph & hg, vector<int> & seeds, unsigned int n, double k, vector<float> & costs, vector<double> & degree, long long int &totalDegree)
{
        long long i;
        unsigned int j,l;
	int maxInd;
        vector<int> e, nList;

        vector<int> nodeDegree(n,0);
        vector<int> indx(n,0);
        for (j = 0; j < n; ++j){
                indx[j] = j;
                nodeDegree[j] = hg.getNode(j+1).size();
        }

        InfCost<int,float> hd(&nodeDegree[0], &costs[0]);
        MappedHeap<InfCost<int,float> > heap(indx,hd);

        long long numEdge = hg.getNumEdge();

        vector<bool> edgeMark(numEdge, false);

        double totalCost = 0;
	degree[0] = 0;

        totalDegree=0;
        i=1;
        while(totalCost < k && !heap.empty()){
                maxInd = heap.pop()+1;

                totalCost+=costs[maxInd-1];
		if (totalCost > k){
			break;
		}

                e = hg.getNode(maxInd);

                degree[i] = degree[i-1]+nodeDegree[maxInd-1];
                totalDegree += nodeDegree[maxInd-1];
		nodeDegree[maxInd-1] = 0;

                seeds.push_back(maxInd);

                for (j = 0; j < e.size(); ++j){
                        if (edgeMark[e[j]]){
                                continue;
                        }

                        nList = hg.getEdge(e[j]);
                        for (l = 0; l < nList.size(); ++l){
				if (nList[l] != maxInd){
	                                nodeDegree[nList[l]-1]--;
        	                        heap.heapify(nList[l]-1);
				}
                        }
                        edgeMark[e[j]] = true;
                }
		i++;
        }
        vector<int>().swap(nodeDegree);
        vector<int>().swap(e);
        vector<int>().swap(nList);
        vector<int>().swap(indx);
        vector<bool>().swap(edgeMark);
}

#endif
