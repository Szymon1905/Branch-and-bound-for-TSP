#include "BB.h"
#include <iostream>
#include <cstring>
#include <vector>

using namespace std;

// zmienne globalne
int liczba_miast;

// Stores the final minimum weight of shortest tour.
int final_res = INT_MAX;

// final_path[] stores the final solution ie, the
// path of the salesman.
vector<int> final_path;

// visited[] keeps track of the already visited nodes
// in a particular path
vector<bool> visited;

void wczytaj_liczbe_miast_BB(int wczytana) {
    liczba_miast = wczytana;
}

void wyzeruj_zmienne() {
    final_path.clear();
    visited.clear();
    final_res = INT_MAX;

}

int counter = 0;


// Function to copy temporary solution to
// the final solution
void copyToFinal(vector<int> curr_path)
{
    for (int i=0; i<curr_path.size()-1; i++)
        final_path[i] = curr_path[i];
    final_path[liczba_miast] = curr_path[0];

    counter++;
    cout<<"new "<<counter<<endl;
    for (int i=0; i<final_path.size(); i++)
        cout<<final_path[i]<<" ";
    cout<<endl;
}

// Function to find the minimum edge cost
// having an end at the vertex i
int firstMin(vector<vector<int>> &macierz, int i)
{
    int min = INT_MAX;
    for (int k=0; k<liczba_miast; k++)
        if (macierz[i][k]<min && i != k)
            min = macierz[i][k];
    return min;
}

// function to find the second minimum edge cost
// having an end at the vertex i
int secondMin(vector<vector<int>> &macierz, int i)
{
    int first = INT_MAX, second = INT_MAX;
    for (int j=0; j<liczba_miast; j++)
    {
        if (i == j)
            continue;

        if (macierz[i][j] <= first)
        {
            second = first;
            first = macierz[i][j];
        }
        else if (macierz[i][j] <= second &&
                macierz[i][j] != first)
            second = macierz[i][j];
    }
    return second;
}

// function that takes as arguments:
// curr_bound -> lower bound of the root node
// curr_weight-> stores the weight of the path so far
// level-> current level while moving in the search
//		 space tree
// curr_path[] -> where the solution is being stored which
//			 would later be copied to final_path[]
void TSPRec(vector<vector<int>> &macierz, int curr_bound, int curr_weight,
            int level, vector<int> curr_path)
{
    // base case is when we have reached level N which
    // means we have covered all the nodes once
    if (level==liczba_miast)
    {
        // check if there is an edge from last vertex in
        // path back to the first vertex
        if (macierz[curr_path[level-1]][curr_path[0]] != 0)
        {
            // curr_res has the total weight of the
            // solution we got
            int curr_res = curr_weight +
                    macierz[curr_path[level-1]][curr_path[0]];

            // Update final result and final path if
            // current result is better.
            if (curr_res < final_res)
            {
                copyToFinal(curr_path);
                final_res = curr_res;
            }
        }
        return;
    }

    // for any other level iterate for all vertices to
    // build the search space tree recursively
    for (int i=0; i<liczba_miast; i++)
    {
        // Consider next vertex if it is not same (diagonal
        // entry in adjacency matrix and not visited
        // already)
        if (macierz[curr_path[level-1]][i] != 0 &&
            visited[i] == false)
        {
            int temp = curr_bound;
            curr_weight += macierz[curr_path[level-1]][i];

            // different computation of curr_bound for
            // level 2 from the other levels
            if (level==1)
                curr_bound -= ((firstMin(macierz, curr_path[level-1]) +
                                firstMin(macierz, i))/2);
            else
                curr_bound -= ((secondMin(macierz, curr_path[level-1]) +
                                firstMin(macierz, i))/2);

            // curr_bound + curr_weight is the actual lower bound
            // for the node that we have arrived on
            // If current lower bound < final_res, we need to explore
            // the node further
            if (curr_bound + curr_weight < final_res)
            {
                curr_path[level] = i;
                visited[i] = true;

                // call TSPRec for the next level
                TSPRec(macierz, curr_bound, curr_weight, level+1,
                       curr_path);
            }

            // Else we have to prune the node by resetting
            // all changes to curr_weight and curr_bound
            curr_weight -= macierz[curr_path[level-1]][i];
            curr_bound = temp;

            // Also reset the visited array
            for (int l = 0; l < visited.size(); l++) {
                visited[l] = false;
            }

            for (int j=0; j<=level-1; j++)
                visited[curr_path[j]] = true;
        }
    }
}

void TSP(vector<vector<int>> &macierz)
{
    vector<int> curr_path(liczba_miast+1, -1);
    // Calculate initial lower bound for the root node
    // using the formula 1/2 * (sum of first min +
    // second min) for all edges.
    // Also initialize the curr_path and visited array
    int curr_bound = 0;


    for (int l = 0; l < curr_path.size()-1; l++) {
        visited.push_back(false);
    }


    // final path to fit to the path at the end with back path
    final_path.resize(liczba_miast+1);

    // Compute initial bound
    for (int i=0; i<liczba_miast; i++)
        curr_bound += (firstMin(macierz, i) +
                       secondMin(macierz, i));

    // Rounding off the lower bound to an integer
    curr_bound = (curr_bound&1)? curr_bound/2 + 1 :
                 curr_bound/2;

    // We start at vertex 1 so the first vertex
    // in curr_path[] is 0
    visited[0] = true;
    curr_path[0] = 0;

    // Call to TSPRec for curr_weight equal to
    // 0 and level 1
    TSPRec(macierz, curr_bound, 0, 1, curr_path);
}

void Branch_and_Bound_start( vector<vector<int>> &macierz ){
    TSP(macierz);

    printf("Minimum cost : %d\n", final_res);
    printf("Path Taken : ");
    for (int i=0; i<=liczba_miast; i++)
        printf("%d ", final_path[i]);
    cout<<endl;
}

vector<int> pobierz_sciezke(){
    return final_path;
}

int pobierz_dlugosc_sciezki(){
    return final_res;
}




