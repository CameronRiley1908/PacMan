#include "Pacman.h"

#include <sstream>
#include <iostream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f), _cPacmanFrameTime(250), _cMunchieFrameTime(500)
{
	_pacman = new Player();
	_pop = new SoundEffect();
	_munchieframeCount = 0;
	_paused = false;
	_pKeyDown = false;
	_pacmanDirection = 0;
	_pacmanCurrentFrameTime = 0;;
	_pacmanFrame = 0;
	_munchieCurrentFrameTime = 0;

	

	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();
	Audio::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();

	if (!Audio::IsInitialised())
	{
		std::cout << "Audio is not initialised" << std::endl;
	}

	if (!_pop->IsLoaded())
	{

		std::cout << "_pop member sound effect has not loaded" << std::endl;
	}
}

Pacman::~Pacman()
{
	delete _pacmanTexture;
	delete _pacmanSourceRect;
	delete _munchieBlueTexture;
	delete _munchieInvertedTexture;
	delete _munchieRect;
	
}

void Pacman::LoadContent()
{
	// Load Pacman
	_pacmanTexture = new Texture2D();
	_pacmanTexture->Load("Textures/Pacman.tga", false);
	_pacmanPosition = new Vector2(350.0f, 350.0f);
	_pacmanSourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Munchie
	_munchieBlueTexture = new Texture2D();
	_munchieBlueTexture->Load("Textures/Munchie.tga", true);
	_munchieInvertedTexture = new Texture2D();
	_munchieInvertedTexture->Load("Textures/MunchieInverted.tga", true);
	_munchieRect = new Rect(100.0f, 450.0f, 12, 12);

	// Load Sound

	_pop->Load("pop.wav");

	

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	// Set Menu Paramters
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
}

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	if (keyboardState->IsKeyDown(Input::Keys::P) && !_pKeyDown) 
	
	{ 
	_pKeyDown = true;
	_paused = !_paused;
	}

	if (keyboardState->IsKeyUp(Input::Keys::P))_pKeyDown = false;


	if (!_paused) {

		// Checks if D key is pressed
		if (keyboardState->IsKeyDown(Input::Keys::D)) {
			_pacmanPosition->X += _cPacmanSpeed * elapsedTime; //Changes Pac-man to move Right
			_pacmanDirection = 0;
		}
			// Checks if A key is pressed
		if (keyboardState->IsKeyDown(Input::Keys::A)) {
			_pacmanPosition->X -= _cPacmanSpeed * elapsedTime; //Changes Pac-man to move left
			_pacmanDirection = 2;
		}

			// Checks if S key is pressed
		if (keyboardState->IsKeyDown(Input::Keys::S)) {
			_pacmanPosition->Y += _cPacmanSpeed * elapsedTime; //Changes Pac-man to move down
			_pacmanDirection = 1;
		}
			// Checks if W key is pressed
		if (keyboardState->IsKeyDown(Input::Keys::W)) {
			_pacmanPosition->Y -= _cPacmanSpeed * elapsedTime; //Changes Pac-man to move up
			_pacmanDirection = 3;
		}

		_pacmanCurrentFrameTime += elapsedTime;

		if (_pacmanCurrentFrameTime > _cPacmanFrameTime)
		{
			_pacmanFrame++;

			if (_pacmanFrame >= 2)

				_pacmanFrame = 0;

			_pacmanCurrentFrameTime = 0;


		}

		_munchieCurrentFrameTime += elapsedTime;

		if (_munchieCurrentFrameTime > _cMunchieFrameTime)
		{
			_munchieframeCount++;

			if (_munchieframeCount >= 2)
				_munchieframeCount = 0;

			_munchieCurrentFrameTime = 0;
		}

		_pacmanSourceRect->X = _pacmanSourceRect->Width * _pacmanFrame;
		_pacmanSourceRect->Y = _pacmanSourceRect->Height * _pacmanDirection;

		if (_pacmanPosition->X + _pacmanSourceRect->Width > 1056)
		{
			Audio::Play(_pop);
			_pacmanPosition->X = 0 - _pacmanSourceRect->Width;
			
		}

		if (_pacmanPosition->X + _pacmanSourceRect->Width < 0)
		{
			Audio::Play(_pop);
			_pacmanPosition->X = 1056 - _pacmanSourceRect->Width;
		
		}

		if (_pacmanPosition->Y + _pacmanSourceRect->Height > 800)
		{
			Audio::Play(_pop);
			_pacmanPosition->Y = 0 - _pacmanSourceRect->Height;
		}

		if (_pacmanPosition->Y + _pacmanSourceRect->Height < 0)
		{
			Audio::Play(_pop);
			_pacmanPosition->Y = 800 - _pacmanSourceRect->Height;
		}
	}
}



void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacmanPosition->X << " Y: " << _pacmanPosition->Y;

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_pacmanTexture, _pacmanPosition, _pacmanSourceRect); // Draws Pacman

	if (_munchieframeCount == 0)
	{
		// Draws Red Munchie
		SpriteBatch::Draw(_munchieInvertedTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

		_munchieframeCount++;
	}
	else
	{
		// Draw Blue Munchie
		SpriteBatch::Draw(_munchieBlueTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
		
		_munchieframeCount++;

		if (_munchieframeCount >= 60)
			_munchieframeCount = 0;
	}
	
	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);


	if (_paused)
	{
		std::stringstream menuStream;
		menuStream << "PAUSED!";

		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);

	}


	SpriteBatch::EndDraw(); // Ends Drawing
}