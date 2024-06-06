#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Audio.hpp>

enum class GameState
{
	MENU,
	DIFFICULTY_SELECTION,
	SINGLE_PLAYER,
	MULTI_PLAYER,
	EXIT
};

enum class Difficulty
{
	EASY,
	MEDIUM,
	HARD
};

class Game {
public:
	Game();
	void run();
	void setupMenu();

	void processEvents();

	void handleKeyboardInput(sf::Keyboard::Key key);

	void setupSounds();

	void setupScoreTexts();

	void update();

	void render();

	void resetGame(sf::CircleShape& ball, sf::RectangleShape& leftPaddle, sf::RectangleShape& rightPaddle, sf::RenderWindow& window, sf::Text& pauseText);

	void setupDifficultyMenu();

	void handleDifficultyMenuInput(sf::Keyboard::Key key);

	void applyDifficultySettings();
private:
	sf::RenderWindow window;
	sf::Font font;
	sf::Text menuText;
	GameState state;
	Difficulty difficulty;

	sf::RectangleShape leftPaddle;
	sf::RectangleShape rightPaddle;
	sf::CircleShape ball;
	sf::Vector2f velocity;
	unsigned int scoreLeft = 0;
	unsigned int scoreRight = 0;
	sf::Text scoreTextLeft, scoreTextRight;
	sf::Text pauseText;
	sf::SoundBuffer scoreBuffer, bounceBuffer;
	sf::Sound scoreSound, bounceSound;

	sf::Text difficultyText;

	const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);

	float aiSpeed = 5;
	float aiReactionSpeed = 5;
	float mistakeProbability = 0.1;
};