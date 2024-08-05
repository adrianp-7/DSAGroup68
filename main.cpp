#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
#include "bridges-cxx-3.4.4-arm64-apple-darwin23.5.0/include/Bridges.h"
#include "bridges-cxx-3.4.4-arm64-apple-darwin23.5.0/include/DataSource.h"
#include "bridges-cxx-3.4.4-arm64-apple-darwin23.5.0/include/MovieActorWikidata.h"
using namespace bridges;

// Quick Sort implementation
void quick_sort(vector<MovieActorWikidata>& arr, int left, int right, bool by_actor, bool ascending) {
    if (left >= right) return;
    int pivot = left + (right - left) / 2;
    auto pivot_value = by_actor ? arr[pivot].getActorName() : arr[pivot].getMovieName();
    int i = left, j = right;
    while (i <= j) {
        while ((ascending ? (by_actor ? arr[i].getActorName() < pivot_value : arr[i].getMovieName() < pivot_value)
                          : (by_actor ? arr[i].getActorName() > pivot_value : arr[i].getMovieName() > pivot_value)))
            i++;
        while ((ascending ? (by_actor ? arr[j].getActorName() > pivot_value : arr[j].getMovieName() > pivot_value)
                          : (by_actor ? arr[j].getActorName() < pivot_value : arr[j].getMovieName() < pivot_value)))
            j--;
        if (i <= j) {
            swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }
    quick_sort(arr, left, j, by_actor, ascending);
    quick_sort(arr, i, right, by_actor, ascending);
}

// Merge Sort implementation
void merge(vector<MovieActorWikidata>& arr, int left, int mid, int right, bool by_actor, bool ascending) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<MovieActorWikidata> L(n1), R(n2);
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if ((ascending ? (by_actor ? L[i].getActorName() <= R[j].getActorName() : L[i].getMovieName() <= R[j].getMovieName())
                       : (by_actor ? L[i].getActorName() >= R[j].getActorName() : L[i].getMovieName() >= R[j].getMovieName()))) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void merge_sort(vector<MovieActorWikidata>& arr, int left, int right, bool by_actor, bool ascending) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    merge_sort(arr, left, mid, by_actor, ascending);
    merge_sort(arr, mid + 1, right, by_actor, ascending);
    merge(arr, left, mid, right, by_actor, ascending);
}


// This program fragment illustrates how to access and read the Wikidata actor/movie data
int main(int argc, char **argv) {

    // create Bridges object
    Bridges bridges(2, "adrianp",
                    "731809136664");

    // set title
    bridges.setTitle("Accessing Wikidata Movie/Actor Data");

    // create data source object
    DataSource ds (&bridges);

    // get the actor movie Wikidata data through the BRIDGES API for 1955.
    // data are available from the early 20th century to now.
    vector<MovieActorWikidata> v = ds.getWikidataActorMovie(1905, 1928);

    // print the count of the records in 1955
    cout << "Data Records from 1905 to 1918: " << v.size() << "\n";


    //TODO: change to work in tandem with the GUI (depending on which button/option the user selects)
    int choice;
    cout << "Choose sorting criteria:\n1. Actor\n2. Movie\n";
    cin >> choice;
    bool by_actor = (choice == 1);

    cout << "Choose sorting order:\n1. Ascending\n2. Descending\n";
    cin >> choice;
    bool ascending = (choice == 1);

    cout << "Choose sorting algorithm:\n1. Quick Sort\n2. Merge Sort\n";
    cin >> choice;
    bool use_quick_sort = (choice == 1);

    if (use_quick_sort) {
        quick_sort(v, 0, v.size() - 1, by_actor, ascending);
    } else {
        merge_sort(v, 0, v.size() - 1, by_actor, ascending);
    }

    // print out the first 20 records of the sorted dataset

    for (int k = 0; k < 20; k++)
        cout << "Actor-Movie Data:" << endl
             << "\tMovie: " << "\"" <<v[k].getMovieName()<< "\"\n"
             << "\tActor: " << "\"" <<v[k].getActorName() << "\"\n";

    return 0;
}