#include "Drawer.h"
#include "File.h"
#include "ValidateInput.h"
#include <string>
#include <vector>
#include <time.h>

void Introduction(Drawer &drawer);
bool Menu(Drawer &drawer);
void PlayGame(std::string SecretWord, Drawer &drawer);
void GetSecretWord(int RawRandomNumber, std::string &SecretWord);
void PrintRules(Drawer &drawer);
std::vector<std::string> ListThePics(std::string FileName);

const unsigned short SCREEN_SPACE = 70;
const unsigned short PADDING = 3;
const char SCREEN_CHARACTER = '|';
const std::string USER_PROMPT = "|   : ";

int main()
{
	// Seed the random number generator with the current time.
	srand((unsigned int)time(NULL));
	Drawer drawer(SCREEN_SPACE, PADDING, SCREEN_CHARACTER, USER_PROMPT);

	// Welcome to player
	Introduction(drawer);

	// Begin the main game loop.
	bool Playing = Menu(drawer);
	while (Playing)
	{
		// Generate a new random number and plug it into the game.
		std::string SecretWord;
		GetSecretWord(rand(), SecretWord);
		PlayGame(SecretWord, drawer);
		Playing = Menu(drawer);
	}
	return(0);
}

void PlayGame(std::string SecretWord, Drawer &drawer)
{
	std::vector<std::string> HangmanPics = ListThePics("HangmanPics.txt");
	std::vector<char> GuessedChars;
	std::string WordGuess;
	for (int WordSize = 0; WordSize < SecretWord.length(); ++WordSize)
	{
		WordGuess.append("_");
	}
	int GuessesAllowed = HangmanPics.size();
	int Guesses = 0;

	while (true)
	{
		drawer.DrawBlankLine();
		drawer.DrawSeperator();
		drawer.DrawBlankLine();
		drawer.DrawMultiLineText(HangmanPics[Guesses]);
		drawer.DrawBlankLine();
		drawer.DrawText(WordGuess);
		drawer.DrawBlankLine();
		drawer.DrawText("Guessed letters: " + std::string(GuessedChars.begin(), GuessedChars.end()));
		drawer.DrawBlankLine();
		drawer.DrawText("Please enter a letter.");
		drawer.DrawUserPrompt();

		// Get a guess, check if it has already been guessed. If it hasn't, add it to already guessed
		// letters and proceed.
		char CharGuess = '#';
		while (true)
		{
			ValidInputChar(CharGuess, USER_PROMPT);
			bool AlreadyGuessed = false;
			for (int i = 0; i < GuessedChars.size(); ++i)
			{
				if (GuessedChars[i] == CharGuess)
				{
					AlreadyGuessed = true;
				}
			}
			if (AlreadyGuessed == true)
			{
				drawer.DrawText("You have already guessed this letter!");
				drawer.DrawUserPrompt();
			}
			else
			{
				GuessedChars.push_back(CharGuess);
				break;
			}
		}

		// Go through the secret word letter by letter to see if the guessed character matches
		// any in the secret word. If the characters match, the same character in the same place
		// replaces the '_' in the WordGuess and a switch is turned to true.
		bool GuessInWord = false;
		for (int index = 0; index < SecretWord.size(); ++index)
		{
			if (SecretWord[index] == CharGuess)
			{
				// Put the letter in the same place in the WordGuess, replacing the '_'
				WordGuess[index] = CharGuess;
				GuessInWord = true;
			}
		}

		// Check the switch to act accordingly as to whether the guess was correct.
		if (!GuessInWord)
		{
			++Guesses;
		}

		// The two end conditions: either the guesses run out or the word has been guessed.
		// Guesses Run Out:
		if (Guesses == GuessesAllowed - 1)
		{
			drawer.DrawSeperator();
			drawer.DrawBlankLine();
			drawer.DrawMultiLineText(HangmanPics[Guesses]);
			drawer.DrawBlankLine();
			drawer.DrawText("You have run out of guesses! The word was " + SecretWord + ".");
			drawer.DrawBlankLine();
			drawer.DrawSeperator();
			break;
		} // Word Has Been Guessed:
		else if (WordGuess == SecretWord)
		{
			drawer.DrawBlankLine();
			drawer.DrawText("Congratulations! You guessed the secret word, " + SecretWord + ".");
			drawer.DrawBlankLine();
			drawer.DrawSeperator();
			break;
		}
	}
}

void GetSecretWord(int RawRandomNumber, std::string &SecretWord)
{
	File file("Wordlist.txt");
	std::string Words = file.GetFileText();

	int NumberOfWords = 0;
	for (auto it = Words.begin() + 1; it != Words.end(); ++it)
	{
		if (*it == '\n')
			++NumberOfWords;
	}
	
	int RandomNumber = RawRandomNumber % NumberOfWords + 1;
	for (auto it1 = Words.begin(); it1 != Words.end(); ++it1)
	{
		if (*it1 == '\n')
			--RandomNumber;
		if (RandomNumber == 0)
		{
			auto it2 = it1;
			while (*(it2 - 1) != '\n')
			{
				--it2;
				if (it2 == Words.begin())
					break;
			}
			SecretWord = std::string(it2, it1);
			break;
		}
	}
}
std::vector<std::string> ListThePics(std::string FileName)
{
	std::vector<std::string> Output;
	File PicFile(FileName);
	std::string PicString = PicFile.GetFileText();
	for (auto it1 = PicString.begin(), it2 = PicString.begin(); it1 != PicString.end(); ++it1)
	{
		if (*it1 == '@')
		{
			Output.push_back(std::string(it2, it1));
			it2 = it1+2; // We don't want the '@' and the '\n'
		}
	}
	return Output;
}

void Introduction(Drawer &drawer)
{
	drawer.DrawSeperator();
	drawer.DrawBlankLine();
	drawer.DrawCentralLine("HANGMAN");
	drawer.DrawBlankLine();
	drawer.DrawSeperator();
	drawer.DrawBlankLine();
	drawer.DrawCentralLine("Welcome to Hangman!!!");
	drawer.DrawBlankLine();
	drawer.DrawBlankLine();
	drawer.DrawCentralLine("A Game By Alex Sigston");
	drawer.DrawBlankLine();
	drawer.DrawSeperator();
}
void PrintRules(Drawer &drawer)
{
	drawer.DrawSeperator();
	drawer.DrawBlankLine();
	drawer.DrawText("In this game, you will have to guess a random word that the computer " 
		"has chosen. Guess a letter, and you will be told if it is in the random word. However, " 
		"beware! If you run out of guesses, you will be hung!");
	drawer.DrawBlankLine();
	drawer.DrawCentralLine("HAVE FUN!!!");
	drawer.DrawBlankLine();
	drawer.DrawSeperator();
}
bool Menu(Drawer &drawer)
{
	int Decision = 0;
	bool PlayGame;

	while (true)
	{
		drawer.DrawBlankLine();
		drawer.DrawText("Would you like to (1) play a game, (2) see the rules, (3) quit the game?");

		// Get a valid decision
		while (Decision < 1 || Decision > 3)
		{
			drawer.DrawUserPrompt();
			ValidInputInt(Decision, USER_PROMPT);
		}

		// Do the right thing
		switch (Decision)
		{
		case 3:
			PlayGame = false;
			break;
		case 2:
			PrintRules(drawer);
			break;
		case 1:
			PlayGame = true;
			break;
		}
		if (Decision == 1 || Decision == 3)
			break;
		Decision = 0;
	}
	return(PlayGame);
}
