#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <iostream>


class Menu {
public:
	sf::Text text;
	sf::RenderWindow* menu;
	Menu() {
		sf::RenderWindow meni(sf::VideoMode(1920, 1080), "Liar's Dice Menu", sf::Style::Default);
		menu = &meni;
		sf::Texture background_texture;
		background_texture.loadFromFile("loading_screen.jpg");
		sf::Sprite background;
		background.setTexture(background_texture);
		background.setPosition(0, 0);
		sf::Font font;
		font.loadFromFile("Cumiskat.ttf");
		text.setFont(font);
		text.setString("Play");
		text.setCharacterSize(64);
		text.setFillColor(sf::Color::Black);

		menu->draw(text);


	}
};
