#ifndef TEST_GUI_GUI_H
#define TEST_GUI_GUI_H

#include <SFML/Graphics.hpp>
#include <iostream>
#pragma once


class Button {
    private:
        sf::RectangleShape buttonShape;
        sf::Text buttonText;
        sf::Font font;
        bool selected = false;

    public:
        Button(float x, float y, float width, float height, sf::Color color, int textSize, sf::String text);
        void draw(sf::RenderWindow& window);
        bool getSelect();
        void setSelect(bool condition);
};



class Scrollbar {
    private:
        sf::RectangleShape bar;
        sf::RectangleShape barBackground;
        bool isScrolling;
        float scrollChange;
        float barBackroundWidth;
        float barBackroundHeight;
        float barHeight;
        float scrollPercentage;

    public:
        Scrollbar(float x, float y, float width, float height);

        void draw(sf::RenderWindow& window);

        void update(float delta);
        void scroll(sf::Event& event, sf::RenderWindow& window);

        float getScrollPercentage();

};

class SelectScreenGui {
    private:
        std::vector<Button> buttons;
        Button mergeButton;
        Button quickButton;
        Button sortButton;
        sf::RectangleShape mergeBorder;
        sf::RectangleShape quickBorder;
        sf::RectangleShape sortBorder;
        Button actorButton;
        Button movieButton;
        sf::RectangleShape actorBorder;
        sf::RectangleShape movieBorder;
        Button ascendingButton;
        Button descendingButton;
        sf::RectangleShape ascendingBorder;
        sf::RectangleShape descendingBorder;


    public:
        SelectScreenGui();
        void addButton(Button button);
        std::vector<Button>& getButtons();

        std::vector<bool> getParam(std::vector<Button>& b);

        void draw(sf::RenderWindow& window);
        void click(const sf::Event& event, sf::RenderWindow& window);

        bool isSortPressed();
        void reset();
};

class SortScreenGui {
    private:
        std::vector<Button> buttons;
        Scrollbar scrollbar;
        Button backButton;
        bool back;

    public:
        SortScreenGui();

        void click(sf::Event& event, sf::RenderWindow& window);
        void scroll(sf::Event& event, sf::RenderWindow& window);
        float getScrollPercentage();

        void draw(sf::RenderWindow& window);
        bool goBack();
        void reset();

};




#endif //TEST_GUI_GUI_H
