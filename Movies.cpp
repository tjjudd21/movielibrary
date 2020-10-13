/*
	Title:  Movies.cpp
	Author:  April R Crockett
	Modified by: Trevor Judd
	Purpose:  Be able to create, manage, print, save & delete a movie library
*/
#include "Movies.h"
#include "Movie.h"
#include "LinkedList.h"
#include "Timer.h"
#include <string>

Movies::Movies()
{
	ListOfMovies = new LinkedList<Movie*>();
}


Movies::~Movies()
{
	delete ListOfMovies;
}

void Movies::addMovieToList() 
{
	char tempString[100];
	int length, year, numOscars;
	double numStars;
	Text* title;
	Text* genre;
	Text* rating;
	int moviePosition;
	
	//add a movie
	cin.ignore();  //remove the \n from the keyboard buffer
	cout << "\n\nMOVIE TITLE: ";
	cin.getline(tempString, 100);
	
	title = new Text(tempString);
	
	cout << "\nMOVIE LENGTH (in minutes): ";
	cin >> length;
	cout << "\nMOVIE YEAR: ";
	cin >> year;
	cin.ignore();
	cout << "\nMOVIE GENRE: ";
	cin.getline(tempString, 100);
	genre = new Text(tempString);
	cout << "\nMOVIE RATING: ";
	cin.getline(tempString, 100);
	rating = new Text(tempString);
	cout << "\nNUMBER OF OSCARS WON: ";
	cin >> numOscars;
	cout << "\nSTAR RATING (out of 10): ";
	cin >> numStars;
	
	//create the movie
	Movie* oneMovie = new Movie(title, length, year, genre, rating, numOscars, numStars);
		
	//add the movie to the list
	ListOfMovies->appendNode(oneMovie);
		
	cout << "\n" << title->getText() << " has been added to the movie library." << endl;
}

void Movies::removeMovieFromList()
{
	int movieChoice;
	
	if(ListOfMovies->getLength() <= 1)
	{
	   cout << endl << "There must always be at least one movie in your library.  You can\'t";
	   cout << " remove any movies right now or you will have no movies in your library.\n";
	}
	else
	{
		cout << "\n\nChoose from the following movies to remove:\n";
		displayMovieTitles();
		cout << "\nChoose a movie to remove between 1 & " << ListOfMovies->getLength() << ":  ";
		cin >> movieChoice;
		
		while(movieChoice < 1 || movieChoice > ListOfMovies->getLength())
		{
			cout << "\nOops!  You must enter a number between 1 & " << ListOfMovies->getLength() << ":  ";
			cin >> movieChoice;
		}

		int movieIndexToBeRemoved = movieChoice-1;
		Text* movieTitle;
		movieTitle = ListOfMovies->getNodeValue(movieIndexToBeRemoved)->getMovieTitle();
		
		cout << "\n\nThe movie \"";
		movieTitle->displayText();
		cout << "\" has been successfully deleted.\n";	
		
		//destroy this movie
		ListOfMovies->deleteNode(movieIndexToBeRemoved);
	}
}



void Movies::editMovieInList()
{
	int movieChoice;
	
	cout << "\n\nChoose from the following movies to edit:\n";
	displayMovieTitles();
	
	cout << "\nChoose a movie to edit between 1 & " << ListOfMovies->getLength() << ":  ";
	cin >> movieChoice;
	
	while(movieChoice < 1 || movieChoice > ListOfMovies->getLength())
	{
		cout << "\nOops!  You must enter a number between 1 & " << ListOfMovies->getLength() << ":  ";
		cin >> movieChoice;
	}
	
	ListOfMovies->getNodeValue(movieChoice - 1)->editMovie();
}



void Movies::displayMovies()
{
	if(ListOfMovies->getLength() > 0)
	{
		for(int x=0; x < (ListOfMovies->getLength()); x++)
		{
			cout << endl << right << setw(50) << "----------MOVIE " << (x+1) << "----------";
			ListOfMovies->getNodeValue(x)->printMovieDetails(); //function is in Movie.cpp
		}
	}
	else	
		cout << "\nThere are no movies in your library yet.";
}

void Movies::displayMovieTitles()
{
	Text* movieTitle;
	
	for(int x=0; x < ListOfMovies->getLength(); x++)
	{
		cout << "\nMOVIE " << (x+1) <<": ";
		movieTitle = ListOfMovies->getNodeValue(x)->getMovieTitle();
		movieTitle->displayText();
	}
}

void Movies::readMoviesFromFile(char *filename)
{
	int numMoviesReadFromFile = 0;
	ifstream inFile;
	char temp[100];
	Text* title;
	Text* genre;
	Text* rating;
	Movie* oneMovie;
	int movieLength; //length of movie in minutes
	int movieYear; //year released
	int movieOscars; //number of oscars won
	float movieNumStars; //from IMDB out of 10 stars
	
	inFile.open(filename);
	if(inFile.good())
	{
		inFile.getline(temp, 100);
		while(!inFile.eof())
		{	
			title = new Text(temp);//create a text for the movie title
			inFile >> movieLength;
			inFile >> movieYear;
			inFile.ignore(); //get rid of \n in the inFile buffer
			
			inFile.getline(temp, 100); //read in genre
			genre = new Text(temp); //create a text for genre
			inFile.getline(temp, 100); //read in rating
			rating = new Text(temp); //create a text for rating
			inFile >> movieOscars;
			inFile >> movieNumStars;
			inFile.ignore(); //get rid of \n in the inFile buffer
			
			//one movie has been read from the file.  Now create a movie object
			oneMovie = new Movie(title, movieLength, movieYear, genre, rating, movieOscars, movieNumStars);
			
			//now add this movie to the library
			ListOfMovies->appendNode(oneMovie);
			
			//confirm addition to user
			cout << endl;
			title->displayText();
			cout << " was added to the movie library!\n";
			
			numMoviesReadFromFile++;						
			inFile.getline(temp, 100); //read in the next movie title if there is one
		}
		//quickSort(0, ListOfMovies->getLength()-1);
		cout << "\n\n" << numMoviesReadFromFile << " movies were read from the file and added to your movie library.\n\n";
	}
	else
	{
		cout << "\n\nSorry, I was unable to open the file.\n";
	}
}

void Movies::saveToFile(char *filename)
{
	ofstream outFile;
	
	outFile.open(filename);
	
	for(int x=0; x < ListOfMovies->getLength(); x++)
	{
		ListOfMovies->getNodeValue(x)->printMovieDetailsToFile(outFile); //function in Movies.cpp
	}
	outFile.close();
	
	cout << "\n\nAll movies in your library have been printed to " << filename << endl;
}


int Movies::getNumMovies() const
{
	return ListOfMovies->getLength();
}

int Movies::binarySearch(Text* movieTitle)
{
	bool found = false;
	int position = -1;
	int first = 0;
	int middle;
	int last = ListOfMovies->getLength() - 1;
	
	while (first <= last && !found)
	{
		middle = (first + last) / 2;
		
		if (strcmp(movieTitle->getText(),ListOfMovies->getNodeValue(middle)->getMovieTitle()->getText()) == 0)
		{
			found = true;
			position = middle;
		}
		else if (strcmp(movieTitle->getText(),ListOfMovies->getNodeValue(middle)->getMovieTitle()->getText()) < 0)
		{
			last = middle - 1;
		}
		else 
		{
			first = middle + 1;  
		}
	}
	return position;
}

void Movies::insertionSort()
{	
	char alfa[100];
	char beta[100];
	int length = ListOfMovies->getLength();
	
	for (int minNode = 1; minNode < length; minNode++)
	{
		for(int i = minNode; i > 0; i--)
		{
			strcpy(alfa, ListOfMovies->getNodeValue(i)->getMovieTitle()->getText());
			strcpy(beta, ListOfMovies->getNodeValue(i - 1)->getMovieTitle()->getText());
			
			if (strcmp(alfa, beta) < 0)
			{
				ListOfMovies->swap(i, i - 1);
			}
		}
	}
}

int Movies::linearSearch(Text* movieTitle)
{
	int index = 0;       
	int position = -1;   
	bool found = false;
	
	while (index < ListOfMovies->getLength() && !found)
	{
		if (strcmp(movieTitle->getText(),ListOfMovies->getNodeValue(index)->getMovieTitle()->getText()) == 0)
		{
			found = true;         
			position = index;     
		}
    index++;                  
	}
   return position;
}

void Movies::bubbleSort()
{
	char alfa[100];
	char beta[100];
	int length = ListOfMovies->getLength();
	
	for (int maxNode = (length - 1); maxNode > 0; maxNode--)
	{
		for(int i = 0; i < maxNode; i++)
		{
			strcpy(alfa, ListOfMovies->getNodeValue(i)->getMovieTitle()->getText());
			strcpy(beta, ListOfMovies->getNodeValue(i + 1)->getMovieTitle()->getText());
			
			if (strcmp(alfa, beta) > 0)
			{
				ListOfMovies->swap(i, i + 1);
			}
		}
	}	
}

void Movies::insertionSortDescending()
{	
	char alfa[100];
	char beta[100];
	int length = ListOfMovies->getLength();
	
	for (int minNode = 1; minNode < length; minNode++)
	{
		for(int i = minNode; i > 0; i--)
		{
			strcpy(alfa, ListOfMovies->getNodeValue(i)->getMovieTitle()->getText());
			strcpy(beta, ListOfMovies->getNodeValue(i - 1)->getMovieTitle()->getText());
			
			if (strcmp(alfa, beta) > 0)
			{
				ListOfMovies->swap(i - 1, i);
			}
		}
	}
}

void Movies::selectionSort()
{
	char alfa[100];
	
	int minIndex;
	Movie* minValue;
	int length = ListOfMovies->getLength();
	
	for(int start = 0; start < (length - 1); start++)
	{
		minIndex = start;
		minValue = ListOfMovies->getNodeValue(start);
		
		for(int i = (start + 1); i < length; i++)
		{
			strcpy(alfa, ListOfMovies->getNodeValue(i)->getMovieTitle()->getText());
			
			if(strcmp(alfa, minValue->getMovieTitle()->getText()) < 0) 
			{
				minValue = ListOfMovies->getNodeValue(start);
				minIndex = i;
			}
		}
		
		ListOfMovies->swap(start, minIndex);
	}
}

void Movies::MergeSort(int first, int last)
{
	int middle;
	
	if (first < last)
	{
		middle = (first+last) / 2;
	
		MergeSort(first, middle);
		MergeSort((middle + 1), last);
		Merge(first, middle, last);
	}
}

void Movies::Merge(int first, int middle, int last)
{	
	int mergedSize = last - first + 1;
	int mergePos = 0;
	int leftPos = 0;
	int rightPos = 0;
	LinkedList<Movie*> *mergedTitles = new LinkedList<Movie*>;
	
	leftPos = first;
	rightPos = middle + 1;
	
	while (leftPos <= middle && rightPos <= last) 
	{
		if (strcmp(ListOfMovies->getNodeValue(leftPos)->getMovieTitle()->getText(), ListOfMovies->getNodeValue(rightPos)->getMovieTitle()->getText()) < 0)
		{
			mergedTitles->insertNode(ListOfMovies->getNodeValue(leftPos), mergePos);
			++leftPos;
		}
		else 
		{
			mergedTitles->insertNode(ListOfMovies->getNodeValue(rightPos), mergePos);
			++rightPos; 
		}
		++mergePos;
	}
	
	while (leftPos <= middle) 
	{
		mergedTitles->insertNode(ListOfMovies->getNodeValue(leftPos), mergePos);
		++leftPos;
		++mergePos;
	}
	
	while (rightPos <= last) 
	{
		mergedTitles->insertNode(ListOfMovies->getNodeValue(rightPos), mergePos);
		++rightPos;
		++mergePos;
	}
	
	for (mergePos = 0; mergePos < mergedSize; ++mergePos) 
	{
		ListOfMovies->deleteNode(first + mergePos);
		ListOfMovies->insertNode(mergedTitles->getNodeValue(mergePos), first + mergePos);
	}
}

void Movies::quickSort(int first, int last)
{
	int middle = 0;
	
	if (first >= last) 
	{
		return;
	}
	
	middle = partition(first, last);
	
	quickSort(first, middle);
	quickSort(middle + 1, last);
}

int Movies::partition(int first, int last)
{
	int low = 0;
	int high = 0;
	int midpoint = 0;
	Movie* pivot;
	int temp = 0;
	bool done = false;
	
	midpoint = first + (last - first) / 2;
	pivot = ListOfMovies->getNodeValue(midpoint);
	
	low = first;
	high = last;
	
	while (!done)
	{
		while (strcmp(ListOfMovies->getNodeValue(low)->getMovieTitle()->getText(), pivot->getMovieTitle()->getText()) < 0) 
		{
			++low;
		}
		
		while (strcmp(pivot->getMovieTitle()->getText(), ListOfMovies->getNodeValue(high)->getMovieTitle()->getText()) < 0) 
		{
			--high;
		}
		
		if (low >= high) 
		{
			done = true;
		}
		
		else 
		{
			ListOfMovies->swap(low, high);
			++low;
			--high;
		}
		
	}
	return high;
}

void Movies::algorithmAnalysis()
{
	time_t start;
	time_t end;
	double elapsedTime;
	
	char tempString[6] = "llama";
	Text* temp = new Text(tempString);
	
	//1
	start = getTime();
	linearSearch(temp);
	end = getTime();
	elapsedTime = totalTime(start, end);
	cout << "\n\tLinear Search elapsed time: " << elapsedTime << " second(s)." << endl;
	
	//2
	start = getTime();
	binarySearch(temp);
	end = getTime();
	elapsedTime = totalTime(start, end);
	cout << "\tBinary Search elapsed time: " << elapsedTime << " second(s)." << endl;
	
	//3
	insertionSortDescending();
	start = getTime();
	bubbleSort();
	end = getTime();
	elapsedTime = totalTime(start, end);
	cout << "\tBubble Sort elapsed time: " << elapsedTime << " second(s)." << endl;
	
	//4
	insertionSortDescending();
	start = getTime();
	selectionSort();
	end = getTime();
	elapsedTime = totalTime(start, end);
	cout << "\tSelection Sort elapsed time: " << elapsedTime << " second(s)." << endl;
	
	//5
	insertionSortDescending();
	start = getTime();
	insertionSort();
	end = getTime();
	elapsedTime = totalTime(start, end);
	cout << "\tInsertion Sort elapsed time: " << elapsedTime << " second(s)." << endl;
	
	//6
	start = getTime();
	insertionSortDescending();
	end = getTime();
	elapsedTime = totalTime(start, end);
	cout << "\tInsertion Sort Descending elapsed time: " << elapsedTime << " second(s)." << endl;
	
	//7
	start = getTime();
	MergeSort(0, ListOfMovies->getLength() - 1);
	end = getTime();
	elapsedTime = totalTime(start, end);
	cout << "\tMerge Sort elapsed time: " << elapsedTime << " second(s)." << endl;
	
	//8
	start = getTime();
	quickSort(0, ListOfMovies->getLength() - 1);
	end = getTime();
	elapsedTime = totalTime(start, end);
	cout << "\tQuick Sort elapsed time: " << elapsedTime << " second(s)." << endl;
	
}