//
// Created by ziv on 20/06/2020.
//

#include "RecommenderSystem.h"


// ___________________________________ define _______________________________________

#define DEFAULT_INIT 0

#define SUCCESS 0
#define FAILURE -1

#define SIMILARITY_RANK_MIN -1
#define USER_RANK_SCALED_MIN -10

#define NOT_FOUND "USER NOT FOUND"
#define FILE_PROB_PREFIX "Unable to open file "

// ___________________________________ using _______________________________________
// using groups: parse, used objects, mathematical and algorithmic

using std::stod;
using std::ifstream;
using std::istringstream;

using std::pair;
using std::string;
using std::vector;
using std::unordered_map;

using std::sort;
using std::sqrt;
using std::accumulate;
using std::inner_product;



// ___________________________________ algo utils _______________________________________

/**
 * calculate the similarity rank
 * using the formula <v1|v2> / (||v1|| * ||v2||)
 * @param vec1 the first vector in the formula (v1)
 * @param vec2 the second vector in the formula (v2)
 * @return the similarity rank of the two vectors
 */
double RecommenderSystem::_similarityRank(const vector<double> &vec1, const vector<double> &vec2)
noexcept
{
	// <v1|v2>
	double numerator = inner_product(vec1.begin(), vec1.end(), vec2.begin(), double(DEFAULT_INIT));
	// ||v1||
	double vec1Norm = sqrt(
			inner_product(vec1.begin(), vec1.end(), vec1.begin(), double(DEFAULT_INIT)));
	// ||v2||
	double vec2Norm = sqrt(
			inner_product(vec2.begin(), vec2.end(), vec2.begin(), double(DEFAULT_INIT)));
	// ||v1|| * ||v2||
	double denominator = vec1Norm * vec2Norm;
	// <v1|v2> / (||v1|| * ||v2||)
	return numerator / denominator;
}

/**
 * calc the representative vector
 * of the user's taste
 * @param userName the name of user
 * to calc it's representative vector
 * @return the representative vector of the user's taste
 */
vector<double> RecommenderSystem::_calcRepresentativeVec(const string &userName) noexcept
{
	// lambda used for the mean calculation
	auto summationLambda = [](const std::size_t previous, const auto &element)
			{return previous + element.second; };

	// calculating the mean of the user's ranks
	double mean = accumulate(_users[userName].begin(), _users[userName].end(),
							 (double) DEFAULT_INIT, summationLambda) /
				  ((double) _users[userName].size());

	// create new vector - the representative vector
	double val;
	vector<double> newVec;
	int len = _movies[_users[userName].begin()->first].size();
	for (int i = 0; i < len; i++)
	{
		val = DEFAULT_INIT;
		for (auto &movieRankPair: _users[userName])
		{
			val += (((double) movieRankPair.second) - mean) *
				   ((double) _movies[movieRankPair.first][i]);
		}
		newVec.push_back(val);
	}
	return newVec;
}

// ___________________________________ queries API _______________________________________

/**
 * find the most recommended movie
 * to the user based on his ranks
 * @param userName the name of user
 * to find the best movie for him
 * @return the name of the best movie
 * for the user or Err message
 * if the user does not exists
 */
string RecommenderSystem::recommendByContent(const string &userName) noexcept(true)
{
	if (_users.count(userName))
	{
		// calc representative vector
		vector<double> representativeVector = _calcRepresentativeVec(userName);

		// find the movie with best similarity rank
		pair<string, double> best(userName, SIMILARITY_RANK_MIN);
		// iterate on teh movies
		for (auto &movie: _movies)
		{
			// if the movie have not been ranked by the user yet
			if (!_users[userName].count(movie.first))
			{
				// calc similarity rank
				double val = _similarityRank(representativeVector, movie.second);

				// if is better change best
				if (val > best.second)
				{
					best.first = movie.first;
					best.second = val;
				}
			}
		}
		return best.first;
	}
	return string(NOT_FOUND);
}

/**
 * find the best movie for the user
 * based on the k most similar
 * movies the user saw
 * @param userName the name of user
 * to find the best movie for him
 * @param k the number of most similar movies
 * @return the name of the best movie
 * for the user or Err message
 * if the user does not exists
 */
string RecommenderSystem::recommendByCF(const string &userName, int k) noexcept(true)
{
	if (_users.count(userName))
	{
		// find the movie with the best rank
		double rank;
		pair<string, double> best(userName, USER_RANK_SCALED_MIN);

		// iterate on all the movies
		for (auto &movieName:_movieNamesOrder)
		{
			// check only for movies which are not yet ranked
			if (!_users[userName].count(movieName))
			{
				// calc expected rank
				rank = predictMovieScoreForUser(movieName, userName, k);

				// if is better change best
				if (rank > best.second)
				{
					best = pair<string, double>(movieName, rank);
				}
			}
		}
		return best.first;
	}
	return string(NOT_FOUND);
}

/**
 *
 * @param movieName the name of the movie to predict for
 * @param userName the name of the user to predict for
 * @param k the number of most similar movies
 * @return the expected ranking the user will give to the movie
 */
double RecommenderSystem::predictMovieScoreForUser(const string &movieName, const string &userName,
												   int k) noexcept(true)
{
	vector<pair<string, double>> bests;

	// iterate on movies which the user have seen before
	for (auto &movie:_users[userName])
	{
		if (!movie.first.empty())
		{
			// calculate the similarity rank
			double relation = _similarityRank(_movies[movieName], _movies[movie.first]);
			bests.emplace_back(movie.first, relation);
		}
	}
	// sort the vector bests using the boolean lambda sortLambda
	auto sortLambda = [](const pair<string, double> &a, const pair<string, double> &b)
			{return (b.second < a.second); };
	sort(bests.begin(), bests.end(), sortLambda);
	// resize it to contain only the k first elements
	// which are the best
	bests.resize(k);

	// calc the expected value of the score the
	// user will rank the movie
	double numerator = DEFAULT_INIT;
	double denominator = DEFAULT_INIT;
	for (auto &movie:bests)
	{
		numerator += movie.second * _users[userName][movie.first];
		denominator += movie.second;
	}
	return numerator / denominator;
}

// ___________________________________ loadData methods _______________________________________

/**
 * load the data to the object from the files
 * @param moviesAttributesFilePath the files
 * to load the movies attributes from
 * @param userRanksFilePath the file to load
 * the users rankings from
 * @return 0 for success -1 for failure
 */
int RecommenderSystem::loadData(const string &moviesAttributesFilePath,
								const string &userRanksFilePath) noexcept(true)
{
	// initialize loading stream
	ifstream loader;

	loader.open(userRanksFilePath);
	if (loader.fail())
	{
		std::cout << FILE_PROB_PREFIX << userRanksFilePath << std::endl;
		return FAILURE;
	}

	// initialize the ordered map of movies
	// in the right order
	// and init the vectors to zero
	string moviesLine = _initMovies(loader);

	// init and fill the unordered map of maps of the users ranks
	_loadUsers(loader, moviesLine);

	loader.close();


	// open and load from the movies characteristics file
	loader.open(moviesAttributesFilePath);
	if (loader.fail())
	{
		std::cout << FILE_PROB_PREFIX << moviesAttributesFilePath << std::endl;
		return FAILURE;
	}

	// fill the vectors for all movies
	_loadMovies(loader);
	loader.close();
	return SUCCESS;
}

/**
 * load the first line from the loader
 * and initializes the _movies map
 * with the names of the movies and
 * init vectors
 * @param loader ifstream object
 * to read all the data needed
 * from the file
 */
string RecommenderSystem::_initMovies(ifstream &loader) noexcept
{
	string currLine, str;
	getline(loader, currLine);
	istringstream line(currLine);
	line >> str;
	while (line)
	{
		_movies[str] = vector<double>();
		_movieNamesOrder.push_back(str);
		line >> str;
	}
	return currLine;
}

/**
 * load the users' ranks for the movies
 * and initializes the users unordered map
 * of undirected maps
 * @param loader ifstream object
 * to read all the data needed
 * from the file
 */
void RecommenderSystem::_loadUsers(ifstream &loader, string &moviesLine) noexcept
{
	string currLine, userName, strVal, movieName;
	// read the file line by line until EOF
	while (getline(loader, currLine))
	{
		unordered_map<string, double> userTmpMap;
		istringstream line(currLine);
		istringstream namesLine(moviesLine);

		line >> userName;
		line >> strVal;
		namesLine >> movieName;
		while (namesLine)
		{
			if (strVal != "NA")
			{
				userTmpMap[movieName] = stod(strVal);
			}
			line >> strVal;
			namesLine >> movieName;
		}
		_users[userName] = userTmpMap;
	}
}

/**
 * load the movies vectors
 * and fill the _movies map
 * with the vectors values
 * @param loader ifstream object
 * to read all the data needed
 * from the file
 */
void RecommenderSystem::_loadMovies(ifstream &loader) noexcept
{
	string currLine, userName, strVal;
	// read the file line by line until EOF
	while (getline(loader, currLine))
	{
		istringstream line(currLine);
		line >> userName;
		line >> strVal;
		while (line)
		{
			_movies[userName].push_back(stod(strVal));
			line >> strVal;
		}
	}
}

