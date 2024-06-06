#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Audio.hpp>
#include "Game.h"
#include <iostream>
#include <random>

Game::Game() : window(sf::VideoMode(1280, 720), "Pong Game Menu"), state(GameState::MENU) {
	window.setFramerateLimit(60);
	window.setSize(sf::Vector2u(1280, 720));
	sf::View view(sf::FloatRect(0, 0, 1280, 720));
	window.setView(view);
	if (!font.loadFromFile("Jersey25Charted-Regular.ttf")) {
		state = GameState::EXIT;
	}

	leftPaddle.setSize(sf::Vector2f(10, 100));
	leftPaddle.setFillColor(sf::Color::White);
	leftPaddle.setPosition(50, 360);

	rightPaddle.setSize(sf::Vector2f(10, 100));
	rightPaddle.setFillColor(sf::Color::White);
	rightPaddle.setPosition(1230, 360);


	ball.setRadius(10);
	ball.setFillColor(sf::Color::White);
	ball.setPosition(395, 295);

	if (sf::Joystick::isConnected(0))
	{
		std::cout << "Controller is connected" << std::endl;
	}

	setupMenu();
	setupSounds();
	setupScoreTexts();
	setupDifficultyMenu();
}

void Game::run()
{
	while (window.isOpen() && state != GameState::EXIT) {
		processEvents();
		update();
		render();
	}
}

void Game::setupMenu() {
	menuText.setFont(font);
	menuText.setString("1. Single Player\n2. Multi Player\nESC to Exit");
	menuText.setCharacterSize(100);
	menuText.setFillColor(sf::Color::White);
	menuText.setPosition(320, 180);
}

void Game::processEvents() {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
		}
		else if (event.type == sf::Event::KeyPressed) {
			handleKeyboardInput(event.key.code);
		}
	}
}

void Game::handleKeyboardInput(sf::Keyboard::Key key) {
	if (key == sf::Keyboard::Escape) 
	{
		window.close();
	}
	else if (state == GameState::MENU) 
	{
		if (key == sf::Keyboard::Num1) 
		{
			state = GameState::DIFFICULTY_SELECTION;
		}
		else if (key == sf::Keyboard::Num2) 
		{
			state = GameState::MULTI_PLAYER;
			ball.setPosition(window.getSize().x / 2, window.getSize().y / 2);
			velocity.x = 0.9f * 7.5;
			velocity.y = 0.9f * 7.5;
		}
		else if (key == sf::Keyboard::Escape)
		{
			window.close();
		}
	}
	else if (state == GameState::DIFFICULTY_SELECTION)
	{
		handleDifficultyMenuInput(key);
	}
}

void Game::setupSounds()
{
	scoreBuffer.loadFromFile("score.wav");
	bounceBuffer.loadFromFile("bounce.wav");
	scoreSound.setBuffer(scoreBuffer);
	bounceSound.setBuffer(bounceBuffer);
}

void Game::setupScoreTexts()
{
	scoreTextLeft.setFont(font);
	scoreTextRight.setFont(font);
	scoreTextLeft.setCharacterSize(30);
	scoreTextRight.setCharacterSize(30);
	scoreTextLeft.setFillColor(sf::Color::Red);
	scoreTextRight.setFillColor(sf::Color::White);
	scoreTextLeft.setPosition(50, 50);
	scoreTextRight.setPosition(1230, 50);
}

void Game::update()
{
	if (state == GameState::MULTI_PLAYER)
	
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
			leftPaddle.setPosition(leftPaddle.getPosition().x, window.getSize().y - leftPaddle.getSize().y);
			std::cout << "Forced Bottom Position Set: " << leftPaddle.getPosition().y << std::endl;
		}

		ball.move(velocity.x, velocity.y);

		if (ball.getPosition().y < 0 || ball.getPosition().y + ball.getRadius() * 2 > window.getSize().y)
		{
			velocity.y = -velocity.y;
		}

		if (ball.getPosition().x < 0 || ball.getPosition().x + ball.getRadius() * 2 > window.getSize().x)
		{
			velocity.x = -velocity.x;
		}

		if (ball.getGlobalBounds().intersects(leftPaddle.getGlobalBounds()) || ball.getGlobalBounds().intersects(rightPaddle.getGlobalBounds()))
		{
			velocity.x = -velocity.x;
			bounceSound.play();
		}

		if (leftPaddle.getPosition().y < 0)
		{
			leftPaddle.setPosition(50, 0);
		}

		if (leftPaddle.getPosition().y + leftPaddle.getSize().y > window.getSize().y) {
			leftPaddle.setPosition(leftPaddle.getPosition().x, window.getSize().y - leftPaddle.getSize().y);
		}

		if (rightPaddle.getPosition().y < 0)
		{
			rightPaddle.setPosition(1230, 0);
		}

		if (rightPaddle.getPosition().y + rightPaddle.getSize().y > window.getSize().y) {
			rightPaddle.setPosition(rightPaddle.getPosition().x, window.getSize().y - rightPaddle.getSize().y);
		}

		if (sf::Joystick::isConnected(0))
		{
			float y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);

			if (y > 15)
			{
				leftPaddle.move(0, 5);
			}
			else if (y < -15)
			{
				leftPaddle.move(0, -5);
			}
		}

		if (sf::Joystick::isConnected(1))
		{
			float y = sf::Joystick::getAxisPosition(1, sf::Joystick::Y);

			if (y > 15)
			{
				rightPaddle.move(0, 5);
			}
			else if (y < -15)
			{
				rightPaddle.move(0, -5);
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && leftPaddle.getPosition().y > 0) {
			leftPaddle.move(0, -5);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			if (leftPaddle.getPosition().y + leftPaddle.getSize().y < window.getSize().y) {
				leftPaddle.move(0, 5);
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && rightPaddle.getPosition().y > 0) {
			rightPaddle.move(0, -5);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			if (rightPaddle.getPosition().y + rightPaddle.getSize().y < window.getSize().y - 1) {
				rightPaddle.move(0, 5);
			}
		}

		if (ball.getPosition().x < 0)
		{
			scoreRight++;
			scoreSound.play();
			resetGame(ball, leftPaddle, rightPaddle, window, pauseText);
			velocity.x = -velocity.x;
		}

		if (ball.getPosition().x + ball.getRadius() * 2 > window.getSize().x)
		{
			scoreLeft++;
			scoreSound.play();
			resetGame(ball, leftPaddle, rightPaddle, window, pauseText);
			velocity.x = -velocity.x;
		}

		scoreTextLeft.setString(std::to_string(scoreLeft));
		scoreTextRight.setString(std::to_string(scoreRight));

		std::cout << "Left Paddle Y Position: " << leftPaddle.getPosition().y << std::endl;
		std::cout << "Right Paddle Y Position: " << rightPaddle.getPosition().y << std::endl;
		std::cout << "Window Height: " << window.getSize().y << std::endl;
		std::cout << "Paddle Height: " << leftPaddle.getSize().y << std::endl;
	}
	else if (state == GameState::SINGLE_PLAYER)
	{
		ball.move(velocity.x, velocity.y);

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(0.0, 1.0);

		bool makeMistake = (dis(gen) < mistakeProbability);

		if (velocity.x > 0)
		{
			float timeToReachPaddle = (rightPaddle.getPosition().x - ball.getPosition().x) / velocity.x;
			float predictedYPosition = ball.getPosition().y + velocity.y * timeToReachPaddle;

			if (makeMistake)
			{
				std::uniform_int_distribution<> moveDecision(0, 2);
				int mistakeType = moveDecision(gen);

				switch (mistakeType)
				{
				case 0: // Delayed Reaction
					break;
				case 1: // OverCorrection
					predictedYPosition += (predictedYPosition > rightPaddle.getPosition().y) ? -50 : 50;
					break;
				case 2: // Random movement
					std::uniform_real_distribution<> randomMove(-1.0, 1.0);
					rightPaddle.move(0, randomMove(gen) * 10);
					break;
				}

			}
			else 
			{
				if (predictedYPosition > rightPaddle.getPosition().y + rightPaddle.getSize().y / 2)
				{
					rightPaddle.move(0, aiReactionSpeed);
				}
				else if (predictedYPosition < rightPaddle.getPosition().y + rightPaddle.getSize().y / 2)
				{
					rightPaddle.move(0, -aiReactionSpeed);
				}
			}
		}

		if (ball.getPosition().y < 0 || ball.getPosition().y + ball.getRadius() * 2 > window.getSize().y)
		{
			velocity.y = -velocity.y;
		}

		if (ball.getPosition().x < 0 || ball.getPosition().x + ball.getRadius() * 2 > window.getSize().x)
		{
			velocity.x = -velocity.x;
		}

		if (ball.getGlobalBounds().intersects(leftPaddle.getGlobalBounds()) || ball.getGlobalBounds().intersects(rightPaddle.getGlobalBounds()))
		{
			velocity.x = -velocity.x;
			bounceSound.play();
		}

		if (leftPaddle.getPosition().y < 0)
		{
			leftPaddle.setPosition(50, 0);
		}

		if (leftPaddle.getPosition().y + leftPaddle.getSize().y > window.getSize().y) {
			leftPaddle.setPosition(leftPaddle.getPosition().x, window.getSize().y - leftPaddle.getSize().y);
		}

		if (sf::Joystick::isConnected(0))
		{
			float y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);

			if (y > 15)
			{
				leftPaddle.move(0, 5);
			}
			else if (y < -15)
			{
				leftPaddle.move(0, -5);
			}
		}

		if (rightPaddle.getPosition().y < 0) {
			rightPaddle.setPosition(rightPaddle.getPosition().x, 0);
		}
		else if (rightPaddle.getPosition().y + rightPaddle.getSize().y > window.getSize().y) {
			rightPaddle.setPosition(rightPaddle.getPosition().x, window.getSize().y - rightPaddle.getSize().y);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && leftPaddle.getPosition().y >= 0)
		{
			leftPaddle.move(0, -5);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && leftPaddle.getPosition().y + leftPaddle.getSize().y <= window.getSize().y)
		{
			leftPaddle.move(0, 5);
		}

		if (ball.getPosition().x < 0)
		{
			scoreRight++;
			scoreSound.play();
			resetGame(ball, leftPaddle, rightPaddle, window, pauseText);
			velocity.x = -velocity.x;
		}

		if (ball.getPosition().x + ball.getRadius() * 2 > window.getSize().x)
		{
			scoreLeft++;
			scoreSound.play();
			resetGame(ball, leftPaddle, rightPaddle, window, pauseText);
			velocity.x = -velocity.x;
		}

		scoreTextLeft.setString(std::to_string(scoreLeft));
		scoreTextRight.setString(std::to_string(scoreRight));
	}
}

void Game::render()
{
	window.clear();
	if (state == GameState::MENU) {
		window.draw(menuText);
	}
	else if ((state == GameState::SINGLE_PLAYER) || (state == GameState::MULTI_PLAYER))
	{
		window.draw(leftPaddle);
		window.draw(rightPaddle);
		window.draw(ball);
		window.draw(scoreTextLeft);
		window.draw(scoreTextRight);
	}
	else if (state == GameState::DIFFICULTY_SELECTION)
	{
		window.draw(difficultyText);
	}
	window.display();
}

void Game::resetGame(sf::CircleShape& ball, sf::RectangleShape& leftPaddle, sf::RectangleShape& rightPaddle, sf::RenderWindow& window, sf::Text& pauseText)
{
	ball.setPosition(window.getSize().x / 2, window.getSize().y / 2);
	leftPaddle.setPosition(50, window.getSize().y / 2 - leftPaddle.getSize().y / 2);
	rightPaddle.setPosition(window.getSize().x - 50 - rightPaddle.getSize().x, window.getSize().y / 2 - rightPaddle.getSize().y / 2);

	velocity = sf::Vector2f(0.9f * 7.5, 0.9f * 7.5);

	pauseText.setString("Game Paused\nPress Enter to continue...");
	pauseText.setPosition(400, 235);
	pauseText.setCharacterSize(50);
	pauseText.setFillColor(sf::Color::White);
	pauseText.setFont(font);
	window.draw(pauseText);
	window.display();

	sf::Event event;
	while (true)
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
			{
				return;
			}
			if (event.type == sf::Event::Closed)
			{
				window.close();
				exit(0);
			}
		}
	}
}

void Game::setupDifficultyMenu()
{
	difficultyText.setString("1. Easy\n2. Medium\n3. Hard");
	difficultyText.setFont(font);
	difficultyText.setCharacterSize(50);
	difficultyText.setPosition(100, 150);
}

void Game::handleDifficultyMenuInput(sf::Keyboard::Key key)
{
	if (state == GameState::DIFFICULTY_SELECTION)
	{
		if (key == sf::Keyboard::Num1)
		{
			difficulty = Difficulty::EASY;
			applyDifficultySettings();
		}
		else if (key == sf::Keyboard::Num2)
		{
			difficulty = Difficulty::MEDIUM;
			applyDifficultySettings();
		}
		else if (key == sf::Keyboard::Num3)
		{
			difficulty = Difficulty::HARD;
			applyDifficultySettings();
		}
	}
}

void Game::applyDifficultySettings()
{
	switch (difficulty)
	{
		case Difficulty::EASY:
			aiSpeed = 5;
			aiReactionSpeed = 6;
			state = GameState::SINGLE_PLAYER;
			ball.setPosition(window.getSize().x / 2, window.getSize().y / 2);
			velocity.x = 0.9f * 7.5;
			velocity.y = 0.9f * 7.5;
			break;
		case Difficulty::MEDIUM:
			aiSpeed = 7.5;
			aiReactionSpeed = 6.5;
			state = GameState::SINGLE_PLAYER;
			ball.setPosition(window.getSize().x / 2, window.getSize().y / 2);
			velocity.x = 0.9f * 7.5;
			velocity.y = 0.9f * 7.5;
			break;
		case Difficulty::HARD:
			aiSpeed = 10;
			aiReactionSpeed = 7;
			state = GameState::SINGLE_PLAYER;
			ball.setPosition(window.getSize().x / 2, window.getSize().y / 2);
			velocity.x = 0.9f * 7.5;
			velocity.y = 0.9f * 7.5;
			break;
	}
}

int main()
{
	Game game;
	game.run();
	return 0;
}