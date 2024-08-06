#include "gui.h"

Button::Button(float x, float y, float width, float height, sf::Color color, int textSize, sf::String text) {
    buttonShape.setPosition(x, y);
    buttonShape.setSize(sf::Vector2f(width, height));
    buttonShape.setFillColor(color);

    if (!font.loadFromFile("AovelSansRounded-rdDL.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
    }

    buttonText.setFont(font);
    buttonText.setString(text);
    buttonText.setCharacterSize(textSize);
    buttonText.setFillColor(sf::Color::Black);

    //center text in box
    sf::FloatRect textRect = buttonText.getLocalBounds();
    buttonText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    buttonText.setPosition(x + width / 2.0f, y + height / 2.0f);

}

void Button::draw(sf::RenderWindow& window) {
    window.draw(buttonShape);
    window.draw(buttonText);
}

void Button::setSelect(bool condition) {
    if (condition) {
        //highlight box with a blue color when clicked and not highlighted
        this->buttonShape.setFillColor(sf::Color(0, 128, 255));
        selected = !selected;
    }
    else {
        //unhighlight box
        this->buttonShape.setFillColor(sf::Color::White);
        selected = !selected;
    }
}

bool Button::getSelect() {
    return selected;
}


Scrollbar::Scrollbar(float x, float y, float width, float height) {
    //load default
    this->barBackroundWidth = width;
    this->barBackroundHeight = height;
    this->scrollPercentage = 0.0f;

    //load in bar shell
    barBackground.setPosition(x, y);
    barBackground.setSize(sf::Vector2f(width, height));
    barBackground.setFillColor(sf::Color(200, 200, 200)); //gray
    barBackground.setOutlineColor(sf::Color::Black);
    barBackground.setOutlineThickness(1.0f);


    //load bar
    bar.setPosition(x, y);
    barHeight = height * (height / 40000);
    bar.setSize(sf::Vector2f(width, barHeight));
    bar.setFillColor(sf::Color::Black);
}

void Scrollbar::draw(sf::RenderWindow& window) {
    window.draw(barBackground);
    window.draw(bar);
}

void Scrollbar::update(float delta) {
    //subtract where bar is on screen by the change from wheel scroll
    scrollPercentage -= delta;

    //check if scroll bar out of bounds
    if (scrollPercentage < 0.0f) {
        scrollPercentage = 0.0f;
    }
    if (scrollPercentage > 1.0f) {
        scrollPercentage = 1.0f;
    }

    //get new y position of bar.
    float barY = barBackground.getPosition().y + (scrollPercentage * (barBackroundHeight - barHeight));
    bar.setPosition(bar.getPosition().x, barY);
}

void Scrollbar::scroll(sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
            update(event.mouseWheelScroll.delta * 0.0001f); // 0.0001f slows scroll speed on screen
        }
    }
    else if (event.type == sf::Event::MouseButtonPressed) {
        if (bar.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
            isScrolling = true;
            scrollChange = (sf::Mouse::getPosition(window).y - bar.getPosition().y); //where drag starts
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased) {
        isScrolling = false;
    }
    else if (event.type == sf::Event::MouseMoved) {
        if (isScrolling) {
            float newY = ((float)sf::Mouse::getPosition(window).y - scrollChange) ;
            float barY = barBackground.getPosition().y; //top of scroll bar box
            float maxY = barY + barBackroundHeight - barHeight;
            std::cout << barY << " " << maxY << std::endl;

            //check if trying to drag outside bounds of scroll bar
            if (newY < barY) {
                newY = barY;
            }
            else if (newY > maxY) {
                newY =  maxY;
            }

            bar.setPosition(bar.getPosition().x, newY);
            scrollPercentage = (newY - barY) / (barBackroundHeight - barHeight);
        }
    }
}

float Scrollbar::getScrollPercentage() {
    return scrollPercentage;
}


SelectScreenGui::SelectScreenGui() :
        mergeButton(175, 150, 200, 50, sf::Color::White, 24, "MERGE SORT"),
        quickButton(425, 150, 200, 50, sf::Color::White, 24, "QUICK SORT"),
        sortButton(300, 450, 200, 50, sf::Color::White, 24, "SORT ITEMS"),
        actorButton(175, 250, 200, 50, sf::Color::White, 24, "ACTOR"),
        movieButton(425, 250, 200, 50, sf::Color::White, 24, "MOVIE"),
        ascendingButton(175, 350, 200, 50, sf::Color::White, 24, "ASCENDING"),
        descendingButton(425, 350, 200, 50, sf::Color::White, 24, "DESCENDING")
        {


    mergeBorder.setPosition(175, 150);
    mergeBorder.setSize(sf::Vector2f(200, 50));
    mergeBorder.setFillColor(sf::Color::Transparent);
    mergeBorder.setOutlineColor(sf::Color::Black);
    mergeBorder.setOutlineThickness(1.0f);

    quickBorder.setPosition(425, 150);
    quickBorder.setSize(sf::Vector2f(200, 50));
    quickBorder.setFillColor(sf::Color::Transparent);
    quickBorder.setOutlineColor(sf::Color::Black);
    quickBorder.setOutlineThickness(1.0f);

    sortBorder.setPosition(300, 450);
    sortBorder.setSize(sf::Vector2f(200, 50));
    sortBorder.setFillColor(sf::Color::Transparent);
    sortBorder.setOutlineColor(sf::Color::Black);
    sortBorder.setOutlineThickness(1.0f);

    actorBorder.setPosition(175, 250);
    actorBorder.setSize(sf::Vector2f(200, 50));
    actorBorder.setFillColor(sf::Color::Transparent);
    actorBorder.setOutlineColor(sf::Color::Black);
    actorBorder.setOutlineThickness(1.0f);

    movieBorder.setPosition(425, 250);
    movieBorder.setSize(sf::Vector2f(200, 50));
    movieBorder.setFillColor(sf::Color::Transparent);
    movieBorder.setOutlineColor(sf::Color::Black);
    movieBorder.setOutlineThickness(1.0f);

    ascendingBorder.setPosition(175, 350);
    ascendingBorder.setSize(sf::Vector2f(200, 50));
    ascendingBorder.setFillColor(sf::Color::Transparent);
    ascendingBorder.setOutlineColor(sf::Color::Black);
    ascendingBorder.setOutlineThickness(1.0f);

    descendingBorder.setPosition(425, 350);
    descendingBorder.setSize(sf::Vector2f(200, 50));
    descendingBorder.setFillColor(sf::Color::Transparent);
    descendingBorder.setOutlineColor(sf::Color::Black);
    descendingBorder.setOutlineThickness(1.0f);


    addButton(mergeButton);
    addButton(quickButton);
    addButton(actorButton);
    addButton(movieButton);
    addButton(ascendingButton);
    addButton(descendingButton);
    addButton(sortButton);
}

void SelectScreenGui::addButton(Button button) {
    buttons.push_back(button);
}

void SelectScreenGui::draw(sf::RenderWindow& window) {
    for (auto& button : buttons) {
        button.draw(window);
    }
    window.draw(mergeBorder);
    window.draw(quickBorder);
    window.draw(actorBorder);
    window.draw(movieBorder);
    window.draw(ascendingBorder);
    window.draw(descendingBorder);
    window.draw(sortBorder);

    sf::Font font;
    if (!font.loadFromFile("AovelSansRounded-rdDL.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
    }


    sf::Text welcome;
    sf::Text option1;
    sf::Text option2;
    sf::Text option3;

    welcome.setFont(font);
    option1.setFont(font);
    option2.setFont(font);
    option3.setFont(font);

    welcome.setString("Movie Info Machine");
    welcome.setCharacterSize(36);
    welcome.setFillColor(sf::Color::Black);
    welcome.setStyle(sf::Text::Bold | sf::Text::Underlined);
    welcome.setPosition(250, 50);
    window.draw(welcome);

    option1.setString("Select sort type:");
    option1.setCharacterSize(20);
    option1.setFillColor(sf::Color::Black);
    option1.setPosition(20, 163);
    window.draw(option1);

    option2.setString("Select key to sort:");
    option2.setCharacterSize(20);
    option2.setFillColor(sf::Color::Black);
    option2.setPosition(20, 263);
    window.draw(option2);

    option3.setString("Select data order:");
    option3.setCharacterSize(20);
    option3.setFillColor(sf::Color::Black);
    option3.setPosition(20, 363);
    window.draw(option3);

}

void SelectScreenGui::click(const sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2<int> pos = sf::Mouse::getPosition(window);
        if (pos.x >= 175 && pos.x <= 375 && pos.y >= 150 && pos.y <= 200) { //merge sort button
            // neither button selected
                buttons[0].setSelect(true);
                buttons[1].setSelect(false);
        }
        else if (pos.x >= 425 && pos.x <= 625 && pos.y >= 150 && pos.y <= 200) { //quick sort button
            buttons[1].setSelect(true);
            buttons[0].setSelect(false);
        }
        else if (pos.x >= 300 && pos.x <= 500 && pos.y >= 450 && pos.y <= 500) { //sort button
            if ((buttons[0].getSelect() || buttons[1].getSelect()) && (buttons[2].getSelect() || buttons[3].getSelect()) &&
                    (buttons[4].getSelect() || buttons[5].getSelect()))
                buttons[6].setSelect(true);
        }
        else if (pos.x >= 175 && pos.x <= 375 && pos.y >= 250 && pos.y <= 300) { //actor button
                buttons[2].setSelect(true);
                buttons[3].setSelect(false);
        }
        else if (pos.x >= 425 && pos.x <= 625 && pos.y >= 250 && pos.y <= 300) { //movie button
                buttons[3].setSelect(true);
                buttons[2].setSelect(false);
        }
        else if (pos.x >= 175 && pos.x <= 375 && pos.y >= 350 && pos.y <= 400) { //ascending button
                buttons[4].setSelect(true);
                buttons[5].setSelect(false);
        }
        else if (pos.x >= 425 && pos.x <= 625 && pos.y >= 350 && pos.y <= 400) { //descending sort button
                buttons[5].setSelect(true);
                buttons[4].setSelect(false);
        }
    }
}

bool SelectScreenGui::isSortPressed() {
    return buttons[6].getSelect();
}

void SelectScreenGui::reset() {
    for (Button &b : buttons) {
        b.setSelect(false);
    }
}

std::vector<Button> &SelectScreenGui::getButtons() {
    return buttons;
}

std::vector<bool> SelectScreenGui::getParam(std::vector<Button> &b) {
    std::vector<bool> parameters;
    if (buttons[0].getSelect()) //use merge sort?
        parameters.push_back(true);
    else
        parameters.push_back(false);


    if (buttons[2].getSelect()) //sort by actor?
        parameters.push_back(true);
    else
        parameters.push_back(false);

    if (buttons[4].getSelect()) //in ascending?
        parameters.push_back(true);
    else
        parameters.push_back(false);

    return parameters;


}

SortScreenGui::SortScreenGui() : scrollbar(750, 50, 20, 500), backButton(0, 0, 80, 30, sf::Color::White, 16, "Back") {
    buttons.push_back(backButton);
    back = false;
}




void SortScreenGui::click(sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2<int> pos = sf::Mouse::getPosition(window);
        if (pos.x >= 0 && pos.x <= 80 && pos.y >= 0 && pos.y <= 30) { //back button location
            back = true;
        }
    }
    scrollbar.scroll(event, window);
}

void SortScreenGui::scroll(sf::Event& event, sf::RenderWindow& window) {
    scrollbar.scroll(event, window);
}

float SortScreenGui::getScrollPercentage() {
    return scrollbar.getScrollPercentage();
}

void SortScreenGui::draw(sf::RenderWindow& window) {
    for (auto& button : buttons) {
        button.draw(window);
    }
    scrollbar.draw(window);

//    int maxDataOnScreen = 20; // Number of names to show at once
//    int totalData = avector.size();
//    int maxStartIndex = totalData - maxDataOnScreen;
//
//    int startIndex = getScrollPercentage() * maxStartIndex;
//
//    sf::Font font;
//    if (!font.loadFromFile("AovelSansRounded-rdDL.ttf")) { // Replace with the correct path
//        std::cerr << "Error loading font!" << std::endl;
//        return;
//    }
//
//    for (int i = 0; i < maxDataOnScreen && ((startIndex + i) < totalData); ++i) {
//        sf::Text dataText;
//        dataText.setFont(font);
//        dataText.setString(names[startIndex + i]);
//        dataText.setCharacterSize(24);
//        dataText.setFillColor(sf::Color::Black);
//        dataText.setPosition(50, 50 + i * 26);
//        window.draw(dataText);
//    }
}

bool SortScreenGui::goBack() {
    return back;
}

void SortScreenGui::reset() {
    back = false;
}