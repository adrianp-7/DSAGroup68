#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include "gui.h"
#include "bridges-cxx-3.4.4-arm64-apple-darwin23.5.0/include/Bridges.h"
#include "bridges-cxx-3.4.4-arm64-apple-darwin23.5.0/include/DataSource.h"
#include "bridges-cxx-3.4.4-arm64-apple-darwin23.5.0/include/data_src/MovieActorWikidata.h"
using namespace std;
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

void drawData(sf::RenderWindow& window, const std::vector<MovieActorWikidata>& v, float scrollPercentage) {
    int maxDataOnScreen = 20; // Number of names to show at once
    int totalData = v.size();
    int maxStartIndex = totalData - maxDataOnScreen;

    int startIndex = scrollPercentage * maxStartIndex;

    sf::Font font;
    if (!font.loadFromFile("AovelSansRounded-rdDL.ttf")) { // Replace with the correct path
        std::cerr << "Error loading font!" << std::endl;
        return;
    }

    for (int i = 0; i < maxDataOnScreen && ((startIndex + i) < totalData); ++i) {
        sf::Text dataText;
        dataText.setFont(font);
        dataText.setString(v[startIndex + i].getActorName() + " : " + v[startIndex + i].getMovieName());
        dataText.setCharacterSize(24);
        dataText.setFillColor(sf::Color::Black);
        dataText.setPosition(50, 50 + i * 26);
        window.draw(dataText);
    }
}

int main() {
    // create Bridges object
    Bridges bridges(2, "adrianp","731809136664");

    // set title
    bridges.setTitle("Accessing Wikidata Movie/Actor Data");

    // create data source object
    DataSource ds(&bridges);

    // get the actor movie Wikidata data through the BRIDGES API for 1955.
    // data are available from the early 20th century to now.
    std::vector<MovieActorWikidata> v = ds.getWikidataActorMovie(1905, 1928);

    SelectScreenGui menu;
    SortScreenGui listy;
    bool startScreen = true;


    sf::RenderWindow window(sf::VideoMode(800, 600), "Get Movie Info NOW");
    while (window.isOpen()) {
        if (startScreen) {
            //menu screen
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    return 0;
                } else if (event.type == sf::Event::MouseButtonPressed) {
                    menu.click(event, window);

                    if (menu.isSortPressed()) { //if sort button pressed return true, move to list screen
                        vector<bool> param;
                        param.clear();
                        param = menu.getParam(menu.getButtons()); // {bool merge, bool movie, bool ascending}
                        if (param[0]) {
                            quick_sort(v, 0, v.size() - 1, param[1], param[2]);
                        } else {
                            merge_sort(v, 0, v.size() - 1, param[1], param[2]);
                        }
                        startScreen = false;
                        break;
                    }
                }
            }
            window.clear(sf::Color::White);
            menu.draw(window);
            window.display();
        } else {
            //sorting screen
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    return 0;
                } else if (event.type == sf::Event::MouseButtonPressed) {
                    listy.click(event, window); //check if click happens

                    if (listy.goBack()) { //back button pressed
                        startScreen = true;
                        listy.reset();
                        menu.reset();
                        break;
                    }
                }

                listy.scroll(event, window); //check if scroll bar is in use if event is correct
            }
            window.clear(sf::Color::White);
            listy.draw(window);
            drawData(window, v, listy.getScrollPercentage());
            window.display();
        }
    }

    return 0;
}

