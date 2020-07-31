//
// Created by ziv on 20/06/2020.
//

#ifndef EX5_RECOMMENDERSYSTEM_H
#define EX5_RECOMMENDERSYSTEM_H

// ___________________________________ includes _______________________________________

#include <map>
#include <vector>
#include <string>
#include <unordered_map>

#include <cmath>
#include <numeric>
#include <algorithm>

#include <iostream>
#include <sstream>
#include <fstream>

// ___________________________________ using _______________________________________

using std::map;
using std::vector;
using std::string;
using std::unordered_map;

using std::ifstream;

// ____________________________ class object RecommenderSystem ____________________________

/**
 * this is the RecommenderSystem class
 * using this class' API tou can have
 * a reccomender System to recommend to
 * users movies they would like.
 * there are two ways to do so:
 *       - recommendByContent
 *       - recommendByCF
 */
class RecommenderSystem
{
private:

	// ___________________________________ members  _______________________________________

	// _users[user_name][movie_name] = rank
	unordered_map<string, unordered_map<string, double>> _users;

	unordered_map<string, vector<double>> _movies;

	vector<string> _movieNamesOrder;

	// ___________________________________ loadData helpers _______________________________________

	/**
	 * load the first line from the loader
	 * and initializes the _movies map
	 * with the names of the movies and
	 * init vectors
	 * @param loader ifstream object
	 * to read all the data needed
	 * from the file
	 */
	string _initMovies(ifstream &loader) noexcept;

	/**
	 * load the users' ranks for the movies
	 * and initializes the users unordered map
	 * of undirected maps
	 * @param loader ifstream object
	 * to read all the data needed
	 * from the file
	 */
	void _loadUsers(ifstream &loader, string &moviesLine) noexcept;

	/**
	 * load the movies vectors
	 * and fill the _movies map
	 * with the vectors values
	 * @param loader ifstream object
	 * to read all the data needed
	 * from the file
	 */
	void _loadMovies(ifstream &loader) noexcept;

	// __________________________________ algorithmic helpers ______________________________________

	/**
	 * calculate the similarity rank
	 * using the formula <v1|v2> / (||v1|| * ||v2||)
	 * @param vec1 the first vector in the formula (v1)
	 * @param vec2 the second vector in the formula (v2)
	 * @return the similarity rank of the two vectors
	 */
	static double _similarityRank(const vector<double> &vec1, const vector<double> &vec2) noexcept;

	/**
	 * calc the representative vector
	 * of the user's taste
	 * @param userName the name of user
	 * to calc it's representative vector
	 * @return the representative vector of the user's taste
	 */
	vector<double> _calcRepresentativeVec(const string &userName) noexcept;

public:

	// ___________________________________ API _______________________________________

	/**
	 * find the most recommended movie
	 * to the user based on his ranks
	 * @param userName the name of user
	 * to find the best movie for him
	 * @return the name of the best movie
	 * for the user or Err message
	 * if the user does not exists
	 */
	string recommendByContent(const string &userName) noexcept(true);

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
	string recommendByCF(const string &userName, int k) noexcept(true);

	/**
	 * load the data to the object from the files
	 * @param moviesAttributesFilePath the files
	 * to load the movies attributes from
	 * @param userRanksFilePath the file to load
	 * the users rankings from
	 * @return 0 for success -1 for failure
	 */
	int loadData(const string &moviesAttributesFilePath,
				 const string &userRanksFilePath) noexcept(true);

	/**
	 *
	 * @param movieName the name of the movie to predict for
	 * @param userName the name of the user to predict for
	 * @param k the number of most similar movies
	 * @return the expected ranking the user will give to the movie
	 */
	double
	predictMovieScoreForUser(const string &movieName, const string &userName, int k) noexcept(true);
};


#endif //EX5_RECOMMENDERSYSTEM_H
