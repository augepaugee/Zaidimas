#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cstdlib> // system() funkcijai
#include <SFML/System.hpp>
#include <ctime>
#include <cmath>

enum class MenuState { MAIN_MENU, EXIT };

int auksas = 0, score = 0, upgrade = 0, upgrade1 = 0;
void Fighting(sf::RenderWindow& window);
void runGame(sf::RenderWindow& window, int &auksas);
void Pasiekimai(sf::RenderWindow& window);

void Pasiekimai(sf::RenderWindow& window) {
    // Įkelti tekstūrą
    sf::Texture achievementsTexture;
    if (!achievementsTexture.loadFromFile("C:\\Grajus\\pasiekimai.jpg")) {
        // Handle loading error
    }

    // Sukurti sprite ir priskirti tekstūrą
    sf::Sprite achievementsSprite;
    achievementsSprite.setTexture(achievementsTexture);

    // Gauti lango ir tekstūros dydžius
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = achievementsTexture.getSize();

    // Nustatyti mastelį, kad tekstūra būtų ištempta per visą langą
    achievementsSprite.setScale(
            static_cast<float>(windowSize.x) / textureSize.x,
            static_cast<float>(windowSize.y) / textureSize.y
    );

    // Įkelti šriftą
    sf::Font font;
    if (!font.loadFromFile("C:\\Grajus\\AmaticSC-Regular.ttf")) {
        // Handle loading error
    }

    // Sukurti "Grįžti atgal" mygtuką
    sf::Text backButton("Grizti atgal", font, 50);
    backButton.setFillColor(sf::Color::White);
    backButton.setPosition(windowSize.x / 2 - backButton.getGlobalBounds().width / 2,
                           windowSize.y - backButton.getGlobalBounds().height - 50);

    // Sukurti pasiekimų tekstus
    std::vector<sf::Text> achievementsTexts;
    std::vector<std::string> achievements = {
            "Laimetos kovos: 1",
            "Laimetos kovos: 2",
            "Laimetos kovos: 5",
            "Laimetos kovos: 10",
            "Laimetos kovos: 20"
    };
    std::vector<int> requiredVictories = {1, 2, 5, 10, 20};

    for (size_t i = 0; i < achievements.size(); ++i) {
        std::string text = achievements[i] + (score >= requiredVictories[i] ? "+" : "-");
        sf::Text achievementText(text, font, 40);
        achievementText.setFillColor(sf::Color::White);
        achievementText.setPosition( 250, 175 + i * 60); // Positioning the texts with some spacing
        achievementsTexts.push_back(achievementText);
    }

    // Sukurti kardu patobulinimų tekstus
    std::vector<sf::Text> upgradeTexts;
    std::vector<std::string> upgrades = {
            "Kardu patobulinimai: 1",
            "Kardu patobulinimai: 10",
            "Kardu patobulinimai: 20",
            "Kardu patobulinimai: 50",
            "Kardu patobulinimai: 100"
    };
    std::vector<int> requiredUpgrades = {1, 10, 20, 50, 100};

    for (size_t i = 0; i < upgrades.size(); ++i) {
        std::string text = upgrades[i] + (upgrade >= requiredUpgrades[i] ? "+" : "-");
        sf::Text upgradeText(text, font, 40);
        upgradeText.setFillColor(sf::Color::White);
        upgradeText.setPosition(250, 500 + i * 60); // Positioning the texts with some spacing, starting lower
        upgradeTexts.push_back(upgradeText);
    }

    // Sukurti šarvų patobulinimų tekstus
    std::vector<sf::Text> armorUpgradeTexts;
    std::vector<std::string> armorUpgrades = {
            "Sarvu patobulinimai: 1",
            "Sarvu patobulinimai: 10",
            "Sarvu patobulinimai: 20",
            "Sarvu patobulinimai: 50",
            "Sarvu patobulinimai: 100"
    };
    std::vector<int> requiredArmorUpgrades = {1, 10, 20, 50, 100};

    for (size_t i = 0; i < armorUpgrades.size(); ++i) {
        std::string text = armorUpgrades[i] + (upgrade1 >= requiredArmorUpgrades[i] ? "+" : "-");
        sf::Text armorUpgradeText(text, font, 40);
        armorUpgradeText.setFillColor(sf::Color::White);
        armorUpgradeText.setPosition(windowSize.x - 1000, 175 + i * 60); // Positioning the texts on the right side with some spacing
        armorUpgradeTexts.push_back(armorUpgradeText);
    }

    // Sukurti aukso pasiekimų tekstus
    std::vector<sf::Text> goldTexts;
    std::vector<std::string> goldAchievements = {
            "Auksas: 1000",
            "Auksas: 2000",
            "Auksas: 5000",
            "Auksas: 10000",
            "Auksas: 20000"
    };
    std::vector<int> requiredGold = {1000, 2000, 5000, 10000, 20000};

    for (size_t i = 0; i < goldAchievements.size(); ++i) {
        std::string text = goldAchievements[i] + (auksas >= requiredGold[i] ? "+" : "-");
        sf::Text goldText(text, font, 40);
        goldText.setFillColor(sf::Color::White);
        goldText.setPosition(windowSize.x - 1000, 500 + i * 60); // Positioning the texts on the right side with some spacing
        goldTexts.push_back(goldText);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed) {
                // Patikrinti, ar paspaustas mygtukas "Grįžti atgal"
                if (backButton.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x),
                                                          static_cast<float>(event.mouseButton.y))) {
                    runGame(window, auksas);
                    return;
                }
            }
        }

        // Gauti pelytės poziciją
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f mousePositionF(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));

        // Patikrinti ir atnaujinti "Grįžti atgal" mygtuką
        if (backButton.getGlobalBounds().contains(mousePositionF)) {
            backButton.setFillColor(sf::Color::Red);
        } else {
            backButton.setFillColor(sf::Color::White);
        }

        // Išvalyti langą
        window.clear();

        // Piešti pasiekimų sprite
        window.draw(achievementsSprite);

        // Piešti pasiekimų tekstus
        for (const auto& text : achievementsTexts) {
            window.draw(text);
        }

        // Piešti kardu patobulinimų tekstus
        for (const auto& text : upgradeTexts) {
            window.draw(text);
        }

        // Piešti šarvų patobulinimų tekstus
        for (const auto& text : armorUpgradeTexts) {
            window.draw(text);
        }

        // Piešti aukso pasiekimų tekstus
        for (const auto& text : goldTexts) {
            window.draw(text);
        }

        // Piešti "Grįžti atgal" mygtuką
        window.draw(backButton);

        // Atvaizduoti, kas nupiešta
        window.display();
    }
}
void ShowArmorShop(sf::RenderWindow& window) {
    // Užkraunama background tekstūra
    sf::Texture textureBackground;
    if (!textureBackground.loadFromFile("C:\\Grajus\\sarvu_parduotuve.png")) {
    }
    sf::Sprite spriteBackground(textureBackground);
    spriteBackground.setScale(
            static_cast<float>(window.getSize().x) / textureBackground.getSize().x,
            static_cast<float>(window.getSize().y) / textureBackground.getSize().y
    );

    // Užkraunama tekstūra
    sf::Texture textureNesulankstomumas, textureNesulauzomumas, textureApsauga;
    if (!textureNesulankstomumas.loadFromFile("C:\\Grajus\\nesulankstomumas.png")) {

    }
    if (!textureNesulauzomumas.loadFromFile("C:\\Grajus\\nesulauzomumas.png")) {

    }
    if (!textureApsauga.loadFromFile("C:\\Grajus\\apsauga.png")) {

    }

    // Sukuriami sprites
    sf::Sprite spriteNesulankstomumas(textureNesulankstomumas), spriteNesulauzomumas(textureNesulauzomumas), spriteApsauga(textureApsauga);
    spriteNesulankstomumas.setScale(200.0f / textureNesulankstomumas.getSize().x, 100.0f / textureNesulankstomumas.getSize().y);
    spriteNesulauzomumas.setScale(200.0f / textureNesulauzomumas.getSize().x, 100.0f / textureNesulauzomumas.getSize().y);
    spriteApsauga.setScale(200.0f / textureApsauga.getSize().x, 100.0f / textureApsauga.getSize().y);

    spriteNesulankstomumas.setPosition(400, 200);
    spriteNesulauzomumas.setPosition(400, 400);
    spriteApsauga.setPosition(400, 600);

    // Įkeliamas šriftas
    sf::Font font;
    if (!font.loadFromFile("C:\\Grajus\\AmaticSC-Regular.ttf")) {

    }

    // Sukuriami tekstai
    sf::Text textNesulankstomumas, textNesulauzomumas, textApsauga;
    textNesulankstomumas.setFont(font);
    textNesulankstomumas.setString("Sarvo nesulankstomumas (+10 hp)");
    textNesulankstomumas.setCharacterSize(24);
    textNesulankstomumas.setFillColor(sf::Color::Black);
    textNesulankstomumas.setPosition(400, 170);

    textNesulauzomumas.setFont(font);
    textNesulauzomumas.setString("Sarvo nesulauzomumas (+5 hp)");
    textNesulauzomumas.setCharacterSize(24);
    textNesulauzomumas.setFillColor(sf::Color::Black);
    textNesulauzomumas.setPosition(400, 370);

    textApsauga.setFont(font);
    textApsauga.setString("Sarvo apsauga (+2 hp)");
    textApsauga.setCharacterSize(24);
    textApsauga.setFillColor(sf::Color::Black);
    textApsauga.setPosition(400, 570);

    // Sukuriami mygtukai
    sf::RectangleShape priceButtonNesulankstomumas(sf::Vector2f(200, 50)), priceButtonNesulauzomumas(sf::Vector2f(200, 50)), priceButtonApsauga(sf::Vector2f(200, 50));
    priceButtonNesulankstomumas.setFillColor(sf::Color::Blue);
    priceButtonNesulauzomumas.setFillColor(sf::Color::Blue);
    priceButtonApsauga.setFillColor(sf::Color::Blue);

    priceButtonNesulankstomumas.setPosition(400, 320);
    priceButtonNesulauzomumas.setPosition(400, 520);
    priceButtonApsauga.setPosition(400, 720);

    sf::Text priceButtonTextNesulankstomumas, priceButtonTextNesulauzomumas, priceButtonTextApsauga;
    priceButtonTextNesulankstomumas.setFont(font);
    priceButtonTextNesulankstomumas.setString("Kaina: 300");
    priceButtonTextNesulankstomumas.setCharacterSize(24);
    priceButtonTextNesulankstomumas.setFillColor(sf::Color::White);
    priceButtonTextNesulankstomumas.setPosition(460, 330);

    priceButtonTextNesulauzomumas.setFont(font);
    priceButtonTextNesulauzomumas.setString("Kaina: 200");
    priceButtonTextNesulauzomumas.setCharacterSize(24);
    priceButtonTextNesulauzomumas.setFillColor(sf::Color::White);
    priceButtonTextNesulauzomumas.setPosition(460, 530);

    priceButtonTextApsauga.setFont(font);
    priceButtonTextApsauga.setString("Kaina: 100");
    priceButtonTextApsauga.setCharacterSize(24);
    priceButtonTextApsauga.setFillColor(sf::Color::White);
    priceButtonTextApsauga.setPosition(460, 730);

    // Sukuriamas grįžimo mygtukas
    sf::RectangleShape returnButton(sf::Vector2f(200, 50));
    returnButton.setFillColor(sf::Color::Green);
    returnButton.setPosition(window.getSize().x / 2 - 100, window.getSize().y - 100);

    // Sukuriamas grįžimo mygtuko tekstas
    sf::Text returnButtonText;
    returnButtonText.setFont(font);
    returnButtonText.setString("Grizti");
    returnButtonText.setCharacterSize(24);
    returnButtonText.setFillColor(sf::Color::Black);
    returnButtonText.setPosition(returnButton.getPosition().x + 80, returnButton.getPosition().y + 10);

    // Pagrindinis loop'as
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                if (priceButtonNesulankstomumas.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    if (auksas >= 300) {
                        auksas -= 300;
                        upgrade1 += 10;
                    } else {
                        //Klaida
                    }
                } else if (priceButtonNesulauzomumas.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    if (auksas >= 200) {
                        auksas -= 200;
                        upgrade1 += 5;
                    } else {
                        //Klaida
                    }
                } else if (priceButtonApsauga.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    if (auksas >= 100) {
                        auksas -= 100;
                        upgrade1 += 2;
                    } else {
                        //Klaida
                    }
                } else if (returnButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    runGame(window, auksas);
                    return;
                }
            }
        }

        // Piesimas
        window.clear();
        window.draw(spriteBackground);
        window.draw(spriteNesulankstomumas);
        window.draw(spriteNesulauzomumas);
        window.draw(spriteApsauga);
        window.draw(textNesulankstomumas);
        window.draw(textNesulauzomumas);
        window.draw(textApsauga);
        window.draw(priceButtonNesulankstomumas);
        window.draw(priceButtonTextNesulankstomumas);
        window.draw(priceButtonNesulauzomumas);
        window.draw(priceButtonTextNesulauzomumas);
        window.draw(priceButtonApsauga);
        window.draw(priceButtonTextApsauga);
        window.draw(returnButton);
        window.draw(returnButtonText);
        window.display();
    }
}
void ShowShop(sf::RenderWindow& window);void ShowShop(sf::RenderWindow& window) {
    // Užkraunama background tekstūra
    sf::Texture textureBackground;
    if (!textureBackground.loadFromFile("C:\\Grajus\\kardu_parduotuve.jpg")) {
        //Klaida
    }
    sf::Sprite spriteBackground(textureBackground);
    spriteBackground.setScale(
            static_cast<float>(window.getSize().x) / textureBackground.getSize().x,
            static_cast<float>(window.getSize().y) / textureBackground.getSize().y
    );

    // Užkraunamos tekstūros
    sf::Texture textureAstrumas, textureKietumas, textureSmailumas;
    if (!textureAstrumas.loadFromFile("C:\\Grajus\\astrumas.png")) {
        //Klaida
    }
    if (!textureKietumas.loadFromFile("C:\\Grajus\\kietumas.png")) {
        //Klaida
    }
    if (!textureSmailumas.loadFromFile("C:\\Grajus\\smailumas.png")) {
        //Klaida
    }

    //Sukuriami sprites
    sf::Sprite spriteAstrumas(textureAstrumas), spriteKietumas(textureKietumas), spriteSmailumas(textureSmailumas);
    spriteAstrumas.setScale(200.0f / textureAstrumas.getSize().x, 100.0f / textureAstrumas.getSize().y);
    spriteKietumas.setScale(200.0f / textureKietumas.getSize().x, 100.0f / textureKietumas.getSize().y);
    spriteSmailumas.setScale(200.0f / textureSmailumas.getSize().x, 100.0f / textureSmailumas.getSize().y);

    spriteAstrumas.setPosition(300, 200);
    spriteKietumas.setPosition(300, 400);
    spriteSmailumas.setPosition(300, 600);

    // Įkeliamas šriftas
    sf::Font font;
    if (!font.loadFromFile("C:\\Grajus\\AmaticSC-Regular.ttf")) {
        //Klaida
    }

    // Sukuriami tekstai
    sf::Text textAstrumas, textKietumas, textSmailumas;
    textAstrumas.setFont(font);
    textAstrumas.setString("Kardo astrumas (+10 dmg)" );
    textAstrumas.setCharacterSize(24);
    textAstrumas.setFillColor(sf::Color::Black);
    textAstrumas.setPosition(300, 170);

    textKietumas.setFont(font);
    textKietumas.setString("Kardo kietumas (+5 dmg)");
    textKietumas.setCharacterSize(24);
    textKietumas.setFillColor(sf::Color::Black);
    textKietumas.setPosition(300, 370);

    textSmailumas.setFont(font);
    textSmailumas.setString("Kardo smailumas (+2 dmg)");
    textSmailumas.setCharacterSize(24);
    textSmailumas.setFillColor(sf::Color::Black);
    textSmailumas.setPosition(300, 570);

    // Sukuriami mygtukai
    sf::RectangleShape priceButtonAstrumas(sf::Vector2f(200, 50)), priceButtonKietumas(sf::Vector2f(200, 50)), priceButtonSmailumas(sf::Vector2f(200, 50));
    priceButtonAstrumas.setFillColor(sf::Color::Blue);
    priceButtonKietumas.setFillColor(sf::Color::Blue);
    priceButtonSmailumas.setFillColor(sf::Color::Blue);

    priceButtonAstrumas.setPosition(300, 320);
    priceButtonKietumas.setPosition(300, 520);
    priceButtonSmailumas.setPosition(300, 720);

    sf::Text priceButtonTextAstrumas, priceButtonTextKietumas, priceButtonTextSmailumas;
    priceButtonTextAstrumas.setFont(font);
    priceButtonTextAstrumas.setString("Kaina: 300");
    priceButtonTextAstrumas.setCharacterSize(24);
    priceButtonTextAstrumas.setFillColor(sf::Color::White);
    priceButtonTextAstrumas.setPosition(320, 330);

    priceButtonTextKietumas.setFont(font);
    priceButtonTextKietumas.setString("Kaina: 200");
    priceButtonTextKietumas.setCharacterSize(24);
    priceButtonTextKietumas.setFillColor(sf::Color::White);
    priceButtonTextKietumas.setPosition(320, 530);

    priceButtonTextSmailumas.setFont(font);
    priceButtonTextSmailumas.setString("Kaina: 100");
    priceButtonTextSmailumas.setCharacterSize(24);
    priceButtonTextSmailumas.setFillColor(sf::Color::White);
    priceButtonTextSmailumas.setPosition(320, 730);

    // Sukuriamas grįžimo atgal mygtukas
    sf::RectangleShape returnButton(sf::Vector2f(200, 50));
    returnButton.setFillColor(sf::Color::Green);
    returnButton.setPosition(window.getSize().x / 2 - 100, window.getSize().y - 100);

    // Sukuriamas grįžimo atgal mygtuko tekstas
    sf::Text returnButtonText;
    returnButtonText.setFont(font);
    returnButtonText.setString("Grizti");
    returnButtonText.setCharacterSize(24);
    returnButtonText.setFillColor(sf::Color::Black);
    returnButtonText.setPosition(returnButton.getPosition().x + 80, returnButton.getPosition().y + 10);

    // Pagrindinis ciklas
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                if (priceButtonAstrumas.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    if (auksas >= 300) {
                        auksas -= 300;
                        upgrade += 10;
                    } else {
                        // Truksta aukso
                    }
                } else if (priceButtonKietumas.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    if (auksas >= 200) {
                        auksas -= 200;
                        upgrade += 5;
                    } else {
                        // Truksta aukso
                    }
                } else if (priceButtonSmailumas.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    if (auksas >= 100) {
                        upgrade += 2;
                        auksas -= 100;
                    } else {
                        // Truksta aukso
                    }
                } else if (returnButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    runGame(window, auksas);
                    return;
                }
            }
        }

        // Draw everything
        window.clear();
        window.draw(spriteBackground);
        window.draw(spriteAstrumas);
        window.draw(spriteKietumas);
        window.draw(spriteSmailumas);
        window.draw(textAstrumas);
        window.draw(textKietumas);
        window.draw(textSmailumas);
        window.draw(priceButtonAstrumas);
        window.draw(priceButtonTextAstrumas);
        window.draw(priceButtonKietumas);
        window.draw(priceButtonTextKietumas);
        window.draw(priceButtonSmailumas);
        window.draw(priceButtonTextSmailumas);
        window.draw(returnButton);
        window.draw(returnButtonText);
        window.display();
    }
}
void Fighting(sf::RenderWindow& window) {
    // Užkraunamos bacnkground teksturos
    sf::Texture textureBackground;
    if (!textureBackground.loadFromFile("C:\\Grajus\\koliziejus.jpg")) {

    }
    sf::Sprite spriteBackground(textureBackground);
    spriteBackground.setScale(
            static_cast<float>(window.getSize().x) / textureBackground.getSize().x,
            static_cast<float>(window.getSize().y) / textureBackground.getSize().y
    );

    // Užkraunama pirmojo kovotojo tekstūra
    sf::Texture textureFighter1;
    if (!textureFighter1.loadFromFile("C:\\Grajus\\Kovotojas.png")) {
        //Klaida
    }
    sf::Sprite spriteFighter1(textureFighter1);
    spriteFighter1.setPosition(450, 600);

    // Užkraunama antrojo kovotojo tekstūra
    sf::Texture textureFighter2;
    if (!textureFighter2.loadFromFile("C:\\Grajus\\Kovotojas2.png")) {
        //Klaida
    }
    sf::Sprite spriteFighter2(textureFighter2);
    spriteFighter2.setPosition(900, 600);

    // Užkraunamos atakavimo tekstūros
    sf::Texture textureFighter1Attack;
    if (!textureFighter1Attack.loadFromFile("C:\\Grajus\\Kovotojas1_ataka.png")) {
        //Klaida
    }

    sf::Texture textureFighter2Attack;
    if (!textureFighter2Attack.loadFromFile("C:\\Grajus\\Kovotojas2_ataka.png")) {
        //Klaida
    }

    // Judejimo greitis
    float moveSpeed = 50.0f;

    // Gyvybės
    int health1 = 100 + upgrade1;
    int health2 = 85 + (score * 10);

    // Įkeliamas šriftas
    sf::Font font;
    if (!font.loadFromFile("C:\\Grajus\\AmaticSC-Regular.ttf")) {
        //Klaida
    }

    // Pirmojo kovotojo gyvybės
    sf::Text healthText1;
    healthText1.setFont(font);
    healthText1.setCharacterSize(24);
    healthText1.setFillColor(sf::Color::Red);

    // Antrojo kovotojo gyvybės
    sf::Text healthText2;
    healthText2.setFont(font);
    healthText2.setCharacterSize(24);
    healthText2.setFillColor(sf::Color::Red);

    // Žalos tekstas pirmajam kovotojui
    sf::Text damageText1;
    damageText1.setFont(font);
    damageText1.setCharacterSize(24);
    damageText1.setFillColor(sf::Color::Yellow);

    // Žalos tekstas antrajam kovotojui
    sf::Text damageText2;
    damageText2.setFont(font);
    damageText2.setCharacterSize(24);
    damageText2.setFillColor(sf::Color::Yellow);

    // Mygtukas left
    sf::RectangleShape buttonLeft(sf::Vector2f(100, 50));
    buttonLeft.setFillColor(sf::Color::Blue);
    sf::Text buttonTextLeft;
    buttonTextLeft.setFont(font);
    buttonTextLeft.setString("Left");
    buttonTextLeft.setCharacterSize(24);
    buttonTextLeft.setFillColor(sf::Color::White);
    buttonTextLeft.setPosition(buttonLeft.getPosition().x + 10, buttonLeft.getPosition().y + 10);

    // Mygtukas right
    sf::RectangleShape buttonRight(sf::Vector2f(100, 50));
    buttonRight.setFillColor(sf::Color::Blue);
    sf::Text buttonTextRight;
    buttonTextRight.setFont(font);
    buttonTextRight.setString("Right");
    buttonTextRight.setCharacterSize(24);
    buttonTextRight.setFillColor(sf::Color::White);
    buttonTextRight.setPosition(buttonRight.getPosition().x + 10, buttonRight.getPosition().y + 10);

    // Mygtukas attack
    sf::RectangleShape buttonAttack(sf::Vector2f(100, 50));
    buttonAttack.setFillColor(sf::Color::Red);
    sf::Text buttonTextAttack;
    buttonTextAttack.setFont(font);
    buttonTextAttack.setString("Attack");
    buttonTextAttack.setCharacterSize(24);
    buttonTextAttack.setFillColor(sf::Color::White);
    buttonTextAttack.setPosition(buttonAttack.getPosition().x + 10, buttonAttack.getPosition().y + 10);

    // Sukuriamas atsitiktinio skaičiaus generatorius
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Nurodoma kieno yra ėjimas
    bool isFighter1Turn = true;

    // Sukuriamas atakos animacija ir parodoma padaryta žala
    auto performAttack = [&](sf::Sprite& attacker, sf::Sprite& opponent, sf::Texture& attackTexture, sf::Texture& originalTexture, int& opponentHealth, sf::Text& damageText) {
        attacker.setTexture(attackTexture);
        window.clear();
        window.draw(spriteBackground);
        window.draw(spriteFighter1);
        window.draw(spriteFighter2);
        window.draw(healthText1);
        window.draw(healthText2);
        window.display();
        sf::sleep(sf::seconds(0.5)); // Show attack animation for 0.5 seconds
        attacker.setTexture(originalTexture);

        float distance = std::abs(attacker.getPosition().x - opponent.getPosition().x);
        int damage = 0;
        if (distance <= 250) {
            damage = std::rand() % 51 + upgrade;
            opponentHealth -= damage;
            if (opponentHealth < 0) {
                opponentHealth = 0;
            }
        }
        damageText.setString("Damage: " + std::to_string(damage));
        damageText.setPosition(opponent.getPosition().x, opponent.getPosition().y - 60);
        window.draw(damageText);
        window.display();
        sf::sleep(sf::seconds(0.5)); // Show damage text for 0.5 seconds
    };

    // Pagrindinis ciklas
    while (window.isOpen() && health1 > 0 && health2 > 0) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                if (isFighter1Turn) {
                    if (buttonLeft.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        sf::Vector2f position = spriteFighter1.getPosition();
                        if (position.x - moveSpeed >= 0) {
                            spriteFighter1.move(-moveSpeed, 0);
                            isFighter1Turn = false;
                        }
                    } else if (buttonRight.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        sf::Vector2f position = spriteFighter1.getPosition();
                        if (position.x + spriteFighter1.getGlobalBounds().width + moveSpeed <= window.getSize().x) {
                            spriteFighter1.move(moveSpeed, 0);
                            isFighter1Turn = false;
                        }
                    } else if (buttonAttack.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        performAttack(spriteFighter1, spriteFighter2, textureFighter1Attack, textureFighter1, health2, damageText2);
                        isFighter1Turn = false;
                        // Check if Fighter 2 is defeated
                        if (health2 <= 0) break;
                    }
                } else {
                    if (buttonLeft.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        sf::Vector2f position = spriteFighter2.getPosition();
                        if (position.x - moveSpeed >= 0) {
                            spriteFighter2.move(-moveSpeed, 0);
                            isFighter1Turn = true;
                        }
                    } else if (buttonRight.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        sf::Vector2f position = spriteFighter2.getPosition();
                        if (position.x + spriteFighter2.getGlobalBounds().width + moveSpeed <= window.getSize().x) {
                            spriteFighter2.move(moveSpeed, 0);
                            isFighter1Turn = true;
                        }
                    } else if (buttonAttack.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        performAttack(spriteFighter2, spriteFighter1, textureFighter2Attack, textureFighter2, health1, damageText1);
                        isFighter1Turn = true;
                    }
                }
            }
        }

        if (!isFighter1Turn && health2 > 0) {
            int action = std::rand() % 10;

            if (action < 6) {
                performAttack(spriteFighter2, spriteFighter1, textureFighter2Attack, textureFighter2, health1, damageText1);
            } else if (action < 8) {
                sf::Vector2f position = spriteFighter2.getPosition();
                if (position.x + spriteFighter2.getGlobalBounds().width + moveSpeed <= window.getSize().x) {
                    spriteFighter2.move(moveSpeed, 0);
                }
            } else {
                sf::Vector2f position = spriteFighter2.getPosition();
                if (position.x - moveSpeed >= 0) {
                    spriteFighter2.move(-moveSpeed, 0);
                }
            }

            isFighter1Turn = true;
        }

        // Atnaujinamos gyvybės ir gyvybių pozicija
        healthText1.setString("Gyvybes: " + std::to_string(health1));
        healthText1.setPosition(spriteFighter1.getPosition().x, spriteFighter1.getPosition().y - 30);

        healthText2.setString("Gyvybes: " + std::to_string(health2));
        healthText2.setPosition(spriteFighter2.getPosition().x, spriteFighter2.getPosition().y - 30);

        // Atnaujinamos likusių mygtukų pozicijos
        float buttonYOffset = 0;
        buttonLeft.setPosition(spriteFighter1.getPosition().x - 120, spriteFighter1.getPosition().y + buttonYOffset);
        buttonTextLeft.setPosition(buttonLeft.getPosition().x + 10, buttonLeft.getPosition().y + 10);

        buttonYOffset += 60;

        buttonRight.setPosition(spriteFighter1.getPosition().x - 120, spriteFighter1.getPosition().y + buttonYOffset);
        buttonTextRight.setPosition(buttonRight.getPosition().x + 10, buttonRight.getPosition().y + 10);
        buttonYOffset += 60;

        buttonAttack.setPosition(spriteFighter1.getPosition().x - 120, spriteFighter1.getPosition().y + buttonYOffset);
        buttonTextAttack.setPosition(buttonAttack.getPosition().x + 10, buttonAttack.getPosition().y + 10);

        // Nupiešiamas backgroundas, kovotojai, gyvybių tekstai ir mygtukai
        window.clear();
        window.draw(spriteBackground);
        window.draw(spriteFighter1);
        window.draw(spriteFighter2);
        window.draw(healthText1);
        window.draw(healthText2);
        window.draw(buttonLeft);
        window.draw(buttonTextLeft);
        window.draw(buttonRight);
        window.draw(buttonTextRight);
        window.draw(buttonAttack);
        window.draw(buttonTextAttack);
        window.display();
    }

    // Nurodomas laimėtojas
    if (health1 <= 0) {
        sf::Text winnerText("Kovotojas 2 nugalejo! Tai yra zaidimo pabaiga.", font, 50);
        std::string scoreString = "Jusu rezultatas: " + std::to_string(score);
        sf::Text scoreText(scoreString, font, 50);
        winnerText.setFillColor(sf::Color::Green);
        scoreText.setFillColor(sf::Color::Red);
        winnerText.setPosition(window.getSize().x / 2 - winnerText.getGlobalBounds().width / 2,
                               window.getSize().y / 2 - winnerText.getGlobalBounds().height / 2);
        scoreText.setPosition(window.getSize().x / 2 - winnerText.getGlobalBounds().width / 2,
                              window.getSize().y / 2 - winnerText.getGlobalBounds().height / 2 + 150);
        window.draw(winnerText);
        window.draw(scoreText);
        window.display();
        sf::sleep(sf::seconds(4)); // Wait for 4 seconds
        window.close();
        return;
    } else if (health2 <= 0) {
        sf::Text winnerText("Kovotojas 1 nugalejo!", font, 50);
        winnerText.setFillColor(sf::Color::Green);
        winnerText.setPosition(window.getSize().x / 2 - winnerText.getGlobalBounds().width / 2,
                               window.getSize().y / 2 - winnerText.getGlobalBounds().height / 2);
        window.draw(winnerText);
        window.display();
        score++;
        sf::sleep(sf::seconds(2)); // Wait for 2 seconds
    }
    srand(static_cast<unsigned int>(time(nullptr)));

    auksas += rand() % 301 + 150;
    runGame(window, auksas);
}
void runGame(sf::RenderWindow& window, int &auksas) {
    // Įkelti tekstūrą
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("C:\\Grajus\\zaidimas.jpg")) {
        //Klaida
    }

    // Sukurti sprite ir priskirti tekstūrą
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    // Gauti lango ir tekstūros dydžius
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = backgroundTexture.getSize();

    // Nustatyti mastelį, kad tekstūra būtų ištempta per visą langą
    backgroundSprite.setScale(
            static_cast<float>(windowSize.x) / textureSize.x,
            static_cast<float>(windowSize.y) / textureSize.y
    );

    // Įkelti šriftą
    sf::Font font;
    if (!font.loadFromFile("C:\\Grajus\\AmaticSC-Regular.ttf")) {
        // Handle loading error
    }

    // Sukurti tekstus
    sf::Text leftText("Kardu parduotuve", font, 50); // Kairėje pusėje
    sf::Text centerText("Kovoti", font, 50);         // Viduryje
    sf::Text rightText("Sarvu parduotuve", font, 50);// Dešinėje pusėje
    sf::Text bottomText("Baigti zaidima", font, 50); // Apačioje
    sf::Text upgradesText("Pasiekimai", font, 50); // Aukštai kairėje

    // Sukurti aukso tekstą
    sf::Text goldText("Auksas: " + std::to_string(auksas), font, 50); // Aukštai kairėje
    goldText.setPosition(100, 100);
    goldText.setFillColor(sf::Color::White);

    // Nustatyti tekstų pozicijas
    leftText.setPosition(100, 550);
    centerText.setPosition(windowSize.x / 2 - centerText.getGlobalBounds().width / 2,
                           windowSize.y / 2 - centerText.getGlobalBounds().height / 2);
    rightText.setPosition(windowSize.x - rightText.getGlobalBounds().width - 40, 650);
    bottomText.setPosition(800, 800);
    upgradesText.setPosition(100, 200);

    // Nustatyti tekstų spalvas
    leftText.setFillColor(sf::Color::White);
    centerText.setFillColor(sf::Color::White);
    rightText.setFillColor(sf::Color::White);
    bottomText.setFillColor(sf::Color::White);
    upgradesText.setFillColor(sf::Color::White);

    // Pagrindinis ciklas
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed) {
                // Patikrinti, ar paspaustas mygtukas yra "Kovoti"
                if (centerText.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x),
                                                          static_cast<float>(event.mouseButton.y))) {
                    // kviečiama funkcija "Fighting", perduodami "window" kaip parametrą
                    Fighting(window);
                } else if (bottomText.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x),
                                                                 static_cast<float>(event.mouseButton.y))) {
                    window.close();
                } else if (leftText.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x),
                                                               static_cast<float>(event.mouseButton.y))) {
                    ShowShop(window);
                } else if (rightText.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x),
                                                                static_cast<float>(event.mouseButton.y))) {
                    ShowArmorShop(window);
                } else if (upgradesText.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x),
                                                                   static_cast<float>(event.mouseButton.y))) {
                    Pasiekimai(window);
                }
            }

            // Gauti pelytės poziciją
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            sf::Vector2f mousePositionF(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));

            // Patikrinti ir atnaujinti kairįjį tekstą
            if (leftText.getGlobalBounds().contains(mousePositionF)) {
                leftText.setFillColor(sf::Color::Red);
                leftText.setCharacterSize(60);
                sf::FloatRect leftTextBounds = leftText.getLocalBounds();
                leftText.setOrigin(leftTextBounds.left + leftTextBounds.width / 2.0f,
                                   leftTextBounds.top + leftTextBounds.height / 2.0f);
                leftText.setPosition(100 + leftTextBounds.width / 2.0f, 550 + leftTextBounds.height / 2.0f);
            } else {
                leftText.setFillColor(sf::Color::White);
                leftText.setCharacterSize(50);
                sf::FloatRect leftTextBounds = leftText.getLocalBounds();
                leftText.setOrigin(leftTextBounds.left + leftTextBounds.width / 2.0f,
                                   leftTextBounds.top + leftTextBounds.height / 2.0f);
                leftText.setPosition(100 + leftTextBounds.width / 2.0f, 550 + leftTextBounds.height / 2.0f);
            }

            // Patikrinti ir atnaujinti centrinį tekstą
            if (centerText.getGlobalBounds().contains(mousePositionF)) {
                centerText.setFillColor(sf::Color::Red);
                centerText.setCharacterSize(60);
                sf::FloatRect centerTextBounds = centerText.getLocalBounds();
                centerText.setOrigin(centerTextBounds.left + centerTextBounds.width / 2.0f,
                                     centerTextBounds.top + centerTextBounds.height / 2.0f);
                centerText.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
            } else {
                centerText.setFillColor(sf::Color::White);
                centerText.setCharacterSize(50);
                sf::FloatRect centerTextBounds = centerText.getLocalBounds();
                centerText.setOrigin(centerTextBounds.left + centerTextBounds.width / 2.0f,
                                     centerTextBounds.top + centerTextBounds.height / 2.0f);
                centerText.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
            }

            // Patikrinti ir atnaujinti dešinįjį tekstą
            if (rightText.getGlobalBounds().contains(mousePositionF)) {
                rightText.setFillColor(sf::Color::Red);
                rightText.setCharacterSize(60);
                sf::FloatRect rightTextBounds = rightText.getLocalBounds();
                rightText.setOrigin(rightTextBounds.left + rightTextBounds.width / 2.0f,
                                    rightTextBounds.top + rightTextBounds.height / 2.0f);
                rightText.setPosition(windowSize.x - rightTextBounds.width / 2.0f - 40,
                                      650 + rightTextBounds.height / 2.0f);
            } else {
                rightText.setFillColor(sf::Color::White);
                rightText.setCharacterSize(50);
                sf::FloatRect rightTextBounds = rightText.getLocalBounds();
                rightText.setOrigin(rightTextBounds.left + rightTextBounds.width / 2.0f,
                                    rightTextBounds.top + rightTextBounds.height / 2.0f);
                rightText.setPosition(windowSize.x - rightTextBounds.width / 2.0f - 40,
                                      650 + rightTextBounds.height / 2.0f);
            }

            // Patikrinti ir atnaujinti apatinį tekstą
            if (bottomText.getGlobalBounds().contains(mousePositionF)) {
                bottomText.setFillColor(sf::Color::Red);
                bottomText.setCharacterSize(35);
                sf::FloatRect bottomTextBounds = bottomText.getLocalBounds();
                bottomText.setOrigin(bottomTextBounds.left + bottomTextBounds.width / 2.0f,
                                     bottomTextBounds.top + bottomTextBounds.height / 2.0f);
                bottomText.setPosition(810, 900);
            } else {
                bottomText.setFillColor(sf::Color::White);
                bottomText.setCharacterSize(30);
                sf::FloatRect bottomTextBounds = bottomText.getLocalBounds();
                bottomText.setOrigin(bottomTextBounds.left + bottomTextBounds.width / 2.0f,
                                     bottomTextBounds.top + bottomTextBounds.height / 2.0f);
                bottomText.setPosition(810, 900);
            }

            // Patikrinti ir atnaujinti "Patobulinimai" tekstą
            if (upgradesText.getGlobalBounds().contains(mousePositionF)) {
                upgradesText.setFillColor(sf::Color::Red);
                upgradesText.setCharacterSize(60);
                sf::FloatRect upgradesTextBounds = upgradesText.getLocalBounds();
                upgradesText.setOrigin(upgradesTextBounds.left + upgradesTextBounds.width / 2.0f,
                                       upgradesTextBounds.top + upgradesTextBounds.height / 2.0f);
                upgradesText.setPosition(1300 + upgradesTextBounds.width / 2.0f, 450);
            } else {
                upgradesText.setFillColor(sf::Color::White);
                upgradesText.setCharacterSize(50);
                sf::FloatRect upgradesTextBounds = upgradesText.getLocalBounds();
                upgradesText.setOrigin(upgradesTextBounds.left + upgradesTextBounds.width / 2.0f,
                                       upgradesTextBounds.top + upgradesTextBounds.height / 2.0f);
                upgradesText.setPosition(1300 + upgradesTextBounds.width / 2.0f, 450);
            }

            // Išvalyti langą
            window.clear();

            // Piešti fono sprite
            window.draw(backgroundSprite);

            // Piešti tekstus
            window.draw(leftText);
            window.draw(centerText);
            window.draw(rightText);
            window.draw(bottomText);
            window.draw(upgradesText);

            // Piešti aukso tekstą
            window.draw(goldText);

            // Atvaizduoti, kas nupiešta
            window.display();
        }
    }
}

    int main() {
        int auksas = 0;
        sf::RenderWindow window(sf::VideoMode(1620, 960), "Pagrindinis Meniu");

        sf::Font font;
        if (!font.loadFromFile("C:\\Grajus\\AmaticSC-Regular.ttf")) {
            std::cerr << "Klaida: Nepavyko įkelti šrifto.\n";
            return 1;
        }

        sf::Texture backgroundTexture;
        if (!backgroundTexture.loadFromFile("C:\\Grajus\\gladiatorius.jpg")) {
            std::cerr << "Klaida: Nepavyko įkelti fono paveikslėlio.\n";
            return 1;
        }
        sf::Sprite background(backgroundTexture);
        background.setScale(static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
                            static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y);

        sf::Text title("Gladiatoriai ir Bridzai", font, 150);
        title.setFillColor(sf::Color::White);

        sf::Text play("Pradeti", font, 90);
        play.setFillColor(sf::Color::White);

        sf::Text exit("Iseiti", font, 90);
        exit.setFillColor(sf::Color::White);

        sf::FloatRect titleBounds = title.getLocalBounds();
        title.setPosition((window.getSize().x - titleBounds.width) / 2, 25);

        sf::FloatRect playBounds = play.getLocalBounds();
        play.setPosition((window.getSize().x - playBounds.width) / 2, 300);

        sf::FloatRect exitBounds = exit.getLocalBounds();
        exit.setPosition((window.getSize().x - exitBounds.width) / 2, 450);

        sf::Music music;
        if (!music.openFromFile("C:\\Grajus\\gladiatorius.mp3")) {
            std::cerr << "Klaida: Nepavyko įkelti foninės muzikos.\n";
            return 1;
        }
        music.setLoop(true);
        music.play();

        MenuState currentState = MenuState::MAIN_MENU;

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
                else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    window.close();
                } else if (event.type == sf::Event::MouseButtonPressed) {
                    if (currentState == MenuState::MAIN_MENU) {
                        if (play.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                            std::cout << "Pradedamas zaidimas!\n";
                            runGame(window, auksas); // Čia paleidžiame jūsų žaidimą
                        } else if (exit.getGlobalBounds().contains(
                                window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                            std::cout << "Programa baigia darba!\n";
                            window.close();
                        }
                    }
                }
            }

            music.setVolume(50); // Nustatome garso lygį

            window.clear();
            window.draw(background);

            if (currentState == MenuState::MAIN_MENU) {
                window.draw(title);
                window.draw(play);
                window.draw(exit);

                // Animacijos pagrindinio meniu mygtukams
                if (play.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                    play.setFillColor(sf::Color::Red);
                    play.setCharacterSize(100);
                    sf::FloatRect playBounds = play.getLocalBounds();
                    play.setPosition((window.getSize().x - playBounds.width) / 2, 300);
                } else {
                    play.setFillColor(sf::Color::White);
                    play.setCharacterSize(90);
                    sf::FloatRect playBounds = play.getLocalBounds();
                    play.setPosition((window.getSize().x - playBounds.width) / 2, 300);
                }

                if (exit.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                    exit.setFillColor(sf::Color::Red);
                    exit.setCharacterSize(100);
                    sf::FloatRect exitBounds = exit.getLocalBounds();
                    exit.setPosition((window.getSize().x - exitBounds.width) / 2, 450);
                } else {
                    exit.setFillColor(sf::Color::White);
                    exit.setCharacterSize(90);
                    sf::FloatRect exitBounds = exit.getLocalBounds();
                    exit.setPosition((window.getSize().x - exitBounds.width) / 2, 450);
                }
            }

            window.display();
        }

        return 0;
    }