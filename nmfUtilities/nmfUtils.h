/**
 * @file nmfUtils.h
 * @brief Definition for common typedefs and structs (i.e., non-graphical)
 * @date Dec 8, 2016
 *
 * This file contains typedefs, structs, and functions common to the MultiSpecies
 * applications. The functions that print various structures are located in this
 * file as well.
 */

#pragma once

#include <ctime>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <map>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include <QDir>

#include "nmfUtilsQt.h"
#include "nmfStructsQt.h"
#include "nmfConstantsMSVPA.h"
#include "nmfLogger.h"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/operation.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/multi_array.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm_ext/erase.hpp>


typedef boost::multi_array<double, 3> Boost3DArrayDouble;
typedef boost::multi_array<double, 4> Boost4DArrayDouble;
typedef boost::multi_array<double, 5> Boost5DArrayDouble;

//struct Gradient_Struct {
//    int RunLength;
//    int NumSpecies;
//    int NumParameters;
//    int MaxIterations;
//    int MaxLineSearches;
//    std::string GrowthForm;
//    std::string HarvestForm;
//    std::string CompetitionForm;
//    std::string PredationForm;
//    std::string Algorithm;
//    std::string Minimizer;
//    std::string ObjectiveCriterion;
//    boost::numeric::ublas::matrix<double> ObservedBiomass;
//    boost::numeric::ublas::matrix<double> OutputBiomass;
//    boost::numeric::ublas::matrix<double> Catch;
//    boost::numeric::ublas::vector<double> GrowthRateMin;
//    boost::numeric::ublas::vector<double> GrowthRateMax;
//    boost::numeric::ublas::vector<double> GrowthRateInit;
//    boost::numeric::ublas::vector<double> CarryingCapacityMin;
//    boost::numeric::ublas::vector<double> CarryingCapacityMax;
//    boost::numeric::ublas::vector<double> CarryingCapacityInit;
//    boost::numeric::ublas::vector<double> CompetitionMin;
//    boost::numeric::ublas::vector<double> CompetitionMax;
//    boost::numeric::ublas::vector<double> CompetitionInit;
//    boost::numeric::ublas::vector<double> PredationMin;
//    boost::numeric::ublas::vector<double> PredationMax;
//    boost::numeric::ublas::vector<double> PredationInit;
// //  boost::numeric::ublas::vector<double> CompetitionPosition; // Position of parameters in each row (i.e., per Species)
//};

struct StatStruct {
    std::vector<double> SSresiduals;
    std::vector<double> SSdeviations;
    std::vector<double> SStotals;
    std::vector<double> rsquared;
    std::vector<double> correlationCoeff;
    std::vector<double> aic;
    std::vector<double> rmse;
    std::vector<double> ri;
    std::vector<double> ae;
    std::vector<double> aae;
    std::vector<double> mef;
    std::vector<double> mohnsRhoGrowthRate;
    std::vector<double> mohnsRhoCarryingCapacity;
    std::vector<double> mohnsRhoEstimatedBiomass;
};

/**
 * @brief Structure contains all of the parameters in a particular Model
 */
struct SystemData {
    std::string Name;
    std::string CarryingCapacity;
    std::string GrowthForm;
    std::string HarvestForm;
    std::string CompetitionForm;
    std::string PredationForm;
    std::string Algorithm;
    std::string Minimizer;
    std::string ObjectiveCriterion;
    std::string Scaling;
    int         NumberOfParameters;
    int         StartYear;
    int         NumberOfRuns;
    int         RunLength;
    int         TimeStep;
    int         NumGenerations;
    int         PopulationSize;
    double      MutationRate;
    int         ConvergenceNum;
    int         BeesNumTotal;
    int         BeesNumEliteSites;
    int         BeesNumBestSites;
    int         BeesNumElite;
    int         BeesNumOther;
    int         BeesNumRepetitions;
    int         BeesMaxGenerations;
    float       BeesNeighborhoodSize;
    int         GradMaxIterations;
    int         GradMaxLineSearches;
    int         NLoptUseStopVal;
    int         NLoptUseStopAfterTime;
    int         NLoptUseStopAfterIter;
    double      NLoptStopVal;
    int         NLoptStopAfterTime;
    int         NLoptStopAfterIter;
    int         NLoptNumberOfRuns;
};

/**
 * @brief The data structure used for parameter estimation. It contains the parameter
 * min max limits as well as the input data.
 */
//struct Data_Struct {

//    bool   showDiagnosticChart;

//    bool   NLoptUseStopVal;
//    bool   NLoptUseStopAfterTime;
//    bool   NLoptUseStopAfterIter;
//    double NLoptStopVal;
//    int    NLoptStopAfterTime;
//    int    NLoptStopAfterIter;
//    int    NLoptNumberOfRuns;

//    std::string MultiRunSpeciesFilename;
//    std::string MultiRunModelFilename;
//    std::string MultiRunSetupFilename;
//    int    RunLength;
//    int    NumSpecies;
//    int    NumGuilds;

//    int    BeesMaxGenerations;
//    int    BeesNumTotal;
//    int    BeesNumBestSites;
//    int    BeesNumEliteSites;
//    int    BeesNumElite;
//    int    BeesNumOther;
//    float  BeesNeighborhoodSize;
//    int    BeesNumRepetitions;

//    int    GAGenerations;
//    int    GAConvergence;

//    int         TotalNumberParameters;
//    std::string Benchmark;

//    std::string GrowthForm;
//    std::string HarvestForm;
//    std::string CompetitionForm;
//    std::string PredationForm;

//    std::string EstimationAlgorithm;
//    std::string MinimizerAlgorithm;
//    std::string ObjectiveCriterion;
//    std::string ScalingAlgorithm;

//    std::map<int,std::vector<int> >       GuildSpecies; // List of species numbers that make up guild num
//    std::vector<int>                      GuildNum;     // Specifies which species are members of which guilds
//    boost::numeric::ublas::matrix<double> ObservedBiomassBySpecies;
//    boost::numeric::ublas::matrix<double> ObservedBiomassByGuilds;
//    boost::numeric::ublas::matrix<double> Catch;
//    boost::numeric::ublas::matrix<double> Effort;
//    boost::numeric::ublas::vector<double> InitBiomass;
//    boost::numeric::ublas::vector<double> InitBiomassMin;
//    boost::numeric::ublas::vector<double> InitBiomassMax;
//    boost::numeric::ublas::vector<double> GrowthRate;
//    boost::numeric::ublas::vector<double> GrowthRateMin;
//    boost::numeric::ublas::vector<double> GrowthRateMax;
//    boost::numeric::ublas::vector<double> CarryingCapacity;
//    boost::numeric::ublas::vector<double> CarryingCapacityMin;
//    boost::numeric::ublas::vector<double> CarryingCapacityMax;
//    boost::numeric::ublas::matrix<double> Exploitation;
//    boost::numeric::ublas::vector<double> ExploitationRateMin; // RSK - change this to matrix?
//    boost::numeric::ublas::vector<double> ExploitationRateMax; // RSK - change this to matrix?
//    boost::numeric::ublas::vector<double> Catchability;
//    boost::numeric::ublas::vector<double> CatchabilityMin;
//    boost::numeric::ublas::vector<double> CatchabilityMax;
//    boost::numeric::ublas::vector<double> SurveyQ;
//    boost::numeric::ublas::vector<double> SurveyQMin;
//    boost::numeric::ublas::vector<double> SurveyQMax;
//    std::vector<std::vector<double> >     CompetitionMin;
//    std::vector<std::vector<double> >     CompetitionMax;
//    std::vector<std::vector<double> >     CompetitionBetaSpeciesMin;
//    std::vector<std::vector<double> >     CompetitionBetaSpeciesMax;
//    std::vector<std::vector<double> >     CompetitionBetaGuildsMin;
//    std::vector<std::vector<double> >     CompetitionBetaGuildsMax;
//    std::vector<std::vector<double> >     CompetitionBetaGuildsGuildsMin;
//    std::vector<std::vector<double> >     CompetitionBetaGuildsGuildsMax;
//    std::vector<std::vector<double> >     PredationRhoMin;
//    std::vector<std::vector<double> >     PredationRhoMax;
//    std::vector<std::vector<double> >     PredationHandlingMin;
//    std::vector<std::vector<double> >     PredationHandlingMax;
//    std::vector<double>                   PredationExponentMin;
//    std::vector<double>                   PredationExponentMax;
//    std::vector<double>                   Parameters;
////  boost::numeric::ublas::matrix<double> OutputBiomass;

//    std::vector<nmfStructsQt::EstimateRunBox> EstimateRunBoxes;
//};


/**
 * @brief The MSVPA Scenarios data structure
 */
struct Scenarios {
    int VarF;
    int VarOthPred;
    int VarOthPrey;
    int VarRec;
    int FishAsF;
    std::string Name;
};


namespace nmfUtils {
    /**
     * @brief Checks that there's at least one item in the passed table's field
     * @param logger : application logger to which to post error messages
     * @param table : table being checked
     * @param field : field in table being checked
     * @param dataMap : data structure holding table data after they've been read from file system
     * @param queryStr : query command used to check the field in table being checked
     * @return True if an error was found, False otherwise
     */
    bool checkForError(nmfLogger *logger,
                       std::string table,
                       std::string field,
                       std::map<std::string, std::vector<std::string> > &dataMap,
                       std::string queryStr);
    /**
     * @brief Template function used to clamp a value to a high and low value.
     * @param value : The value to clamp.
     * @param lower : The lower limit to which to clamp the value if it is less than.
     * @param upper : The upper limit to which to clamp the value if it is greater than.
     */
    template<typename T>
    void clamp(T &value, double lower, double upper)
    {
        value = (value <= lower) ? lower : (value >= upper) ? upper : value;
    }
    /**
     * @brief Converts the catch units string description to the appropriate factor value in thousands of fish
     * @param catchUnits : catch units string (i.e., "Millions of Fish")
     * @return The converted value (i.e., for "Millions of Fish" it returns 1000.0)
     */
    double convertCatchUnitsToValue(std::string catchUnits);
    /**
     * @brief Converts the size units string description to the appropriate factor value in centimeters
     * @param sizeUnits : size units string (i.e., "Inches")
     * @return The converted value (i.e., for "Inches" it returns 2.54)
     */
    double convertSizeUnitsToValue(std::string sizeUnits);
    /**
     * @brief Converts the passed value to a scientific notation formatted string
     * @param val : value to convert to scientific notation
     * @return String representing the passed in value in scientific notation
     */
    std::string convertToScientificNotation(double val);
    /**
     * @brief Converts the weight units string description to the appropriate factor value in kilograms
     * @param weightUnits : size units string (i.e., "Gram")
     * @return The converted value (i.e., for "Gram" it returns 0.001)
     */
    double convertWeightUnitsToValue(std::string weightUnits);
    /**
     * @brief Copies the first file contents to the second file
     * @param fileA : file to be copied from
     * @param fileB : file to be copied to
     */
    void copyFile(std::string fileA, std::string fileB);
    /**
     * @brief Returns the files in the passed directory having the given extension
     * @param path : directory in which to search for files
     * @param ext : extension of files to list
     * @param filesToCopy : the list of files having the given extension in the given directory
     */
    void getFilesWithExt(std::string path,
                         std::string ext,
                         QStringList& filesToCopy);
    /**
     * @brief Gets the maximum element value from the passed matrix
     * @param mat : matrix in which to find the maximum value
     * @param roundOff : If True, will round max value off to nearest tens or hundrerds or...place (i.e., 12 -> 10, 180 -> 200)
     * @return The max value in matrix
     */
    double getMatrixMax(boost::numeric::ublas::matrix<double> &mat,
                        bool roundOff);
    /**
     * @brief Get the sum over all of the matrix elements
     * @param mat : matrix in which to sum over
     * @return : The sum over all of the matrix elements
     */
    double getMatrixSum(const boost::numeric::ublas::matrix<double> &mat);
    /**
     * @brief Gets the name of the operating system
     * @return The name of the operating system on which the application is running
     */
    std::string getOS();
    /**
     * @brief Returns a random number between the passed limits: [lowerLimit,upperLimit). The
     * random number is generated using the Mersene Twister 19937 generator (64 bit) algorithm.
     * @param seed : If seed < 0, no seed is used and the algorithm is stochastic, else
     * the seed value is used and the algorithm is deterministic.
     * @param lowerLimit : lower limit of random number range
     * @param upperLimit : upper limit of random number range
     * @return The random number generated using the passed parameters
     */
    double getRandomNumber(int seed,
                           double lowerLimit,
                           double upperLimit);
    /**
     * @brief Get the sum over all of the vector elements
     * @param vec : vector in which to sum over
     * @return : The sum over all of the vector elements
     */
    double getVectorSum(const boost::numeric::ublas::vector<double> &vec);
    /**
     * @brief Initializes an STL vector of doubles
     * @param vec : vector to initialize
     * @param numValues : number of doubles in vector
     */
    void initialize(std::vector<double>& vec, const int& numValues);
    /**
     * @brief Initializes a vector of std::string types
     * @param vec : vector to initialize
     * @param nrows : number of rows in vector
     */
    void initialize(boost::numeric::ublas::vector<std::string>& vec,
                    const int& nrows);
    /**
     * @brief Initializes a vector of int types
     * @param vec : vector to initialize
     * @param nrows : number of rows in vector
     */
    void initialize(boost::numeric::ublas::vector<int>& vec,
                    const int& nrows);
    /**
     * @brief Initializes a vector of double types
     * @param vec : vector to initialize
     * @param nrows : number of rows in vector
     */
    void initialize(boost::numeric::ublas::vector<double>& vec,
                    const int& nrows);
    /**
     * @brief Initializes a matrix of int types
     * @param mat : matrix to initialize
     * @param nrows : number of rows in matrix
     * @param ncols : number of cols in matrix
     */
    void initialize(boost::numeric::ublas::matrix<int>& mat,
                    const int& nrows,
                    const int& ncols);
    /**
     * @brief Initializes a matrix of double types
     * @param mat : matrix to initialize
     * @param nrows : number of rows in matrix
     * @param ncols : number of cols in matrix
     */
    void initialize(boost::numeric::ublas::matrix<double>& mat,
                    const int& nrows,
                    const int& ncols);
    /**
     * @brief Initializes a 3d array of doubles
     * @param array3d : 3d array to initialize
     */
    void initialize(Boost3DArrayDouble& array3d);
    /**
     * @brief Initializes a 4d array of doubles
     * @param array4d : 4d array to initialize
     */
    void initialize(Boost4DArrayDouble& array4d);
    /**
     * @brief Initializes a 5d array of doubles
     * @param array5d : 5d array to initialize
     */
    void initialize(Boost5DArrayDouble& array5d);

    /**
     * @brief Finds the inverse of the passed matrix. (In order for an inverse to be found, the determinant of the matrix must not be 0.)
     * @param matrix : matrix whose inverse is desired
     * @param inverseMatrix : the inverse of the passed in matrix
     * @return True if inverse was found, else False
     */
    bool invertMatrix(
            boost::numeric::ublas::matrix<double>& matrix,
            boost::numeric::ublas::matrix<double>& inverseMatrix);
    /**
     * @brief Checks a list from within the passes Data_Struct to see if a specific parameter has has its checkbox checked on the Estimation Tab6 page
     * @param dataStruct : data structure to check a list for the passed parameter name
     * @param ParameterName : name of parameter to check in list in Data_Struct
     * @return True if parameter should be estimated, false otherwise
     */
    bool isEstimateParameterChecked(
            const nmfStructsQt::ModelDataStruct& dataStruct,
            const std::string& ParameterName);
    /**
     * @brief Tests the passed value if there's no decimal part and returns true, else returns false
     * @param value : number to test if there's a decimal part
     * @return True or False value signifying if the passed number is a "whole" number (i.e., there's no decimal part)
     */
    bool isWholeNumber(double value);
    /**
     * @brief Checks to see if the passed double is very close to 0
     * @param value : value to check to see if close to 0
     * @return boolean true if nearly zero, else false
     */
    bool isNearlyZero(const double& value);
    /**
     * @brief Returns True/False if current operating system is Windows (either 32-bit or 64-bit)
     * @return True or False signifying current operating system is Windows
     */
    bool isOSWindows();
    /**
     * @brief prints out a 3d double array
     * @param name : name of array
     * @param dimensions : dimensions of array
     * @param array : the 3d double array
     * @param width : the number of characters to use for each array element
     * @param precision : the number of decimal places to use for each array element
     */
    void print3DArray(const std::string& name,
                      const std::vector<int>& dimensions,
                      const Boost3DArrayDouble& array,
                      const int& width,
                      const int& precision);
    /**
     * @brief prints out a 4d double array
     * @param name : name of array
     * @param dimensions : dimensions of array
     * @param array : the 4d double array
     */
    void print4DArray(const std::string& name,
                      const std::vector<int>& dimensions,
                      const Boost4DArrayDouble& array);
    /**
     * @brief prints out an error message
     * @param msg : message to print
     * @param errorMsg : previously captured error message to print
     */
    void printError(const std::string msg,
                    const std::string errorMsg);
    /**
     * @brief prints out all of the key value pairs in the passed data map
     * @param name : name of map
     * @param dataMap : map containing std::string keys and vectors of std::string types
     */
    void printMap(const std::string name,
                  const std::map<std::string, std::vector<std::string> >& dataMap);
    /**
     * @brief prints out the std::string and the corresponding vector of std::string values
     * @param pair : includes a std::string and a vector of std::string types
     */
    void printMapPair(const std::pair<std::string, std::vector<std::string> >& pair);
    /**
     * @brief prints out a matrix of double types
     * @param name : name of matrix
     * @param mat : the matrix of double types
     * @param width : the number of characters to use for each array element
     * @param precision : the number of decimal places to use for each array element
     */
    void printMatrix(const std::string& name,
                     const boost::numeric::ublas::matrix<double>& mat,
                     const int width,
                     const int precision);
    /**
     * @brief prints the passed message
     * @param msg : message to be printed
     */
    void printMsg(std::string msg);
    /**
     * @brief Template function used to print out value of any type of variable passed to it.
     * @param name : variable name
     * @param value : variable value
     * @param width : the number of characters to use for each array element
     * @param precision : the number of decimal places to use for each array element
     */
    template<typename T>
    void printValue(std::string name,
                    T value,
                    const int width,
                    const int precision)
    {
        std::cout << name << ": "
                  << std::setw(width)
                  << std::setprecision(precision)
                  << value << std::endl;
    }
    /**
     * @brief prints out the name of a std::vector of std::string types and its elements
     * @param name : name of std::vector
     * @param vec : the std::vector of std::string types to print
     */
    void printVector(const std::string& name,
                     const std::vector<std::string>& vec);
    /**
     * @brief prints out the name of the boost vector of doubles and its elements
     * @param name : name of boost vector
     * @param vec : the boost vector of doubles to print
     */
    void printVector(const std::string& name,
                     const boost::numeric::ublas::vector<double>& vec);
    /**
     * @brief prints out the name of the boost vector of doubles and its elements
     * @param name : name of boost vector
     * @param vec : the boost vector of doubles to print
     */
    void printVector(const std::string& name,
                     const boost::numeric::ublas::vector<std::string>& vec);
    /**
     * @brief reads a table and creates a map of table names as keys and table descriptions as values
     * @param tableName : name of table
     * @param tableDescription : map of table name and table description
     */
    void readTableDescriptions(std::string tableName,
                               std::map<std::string,std::string>& tableDescription);
    /**
     * @brief reads table names into a map where the key is SSVPA or
     * MSVPA and the value is a vector of associated tables
     * @param TableNames : map of vectors of table names binned by SSVPA or MSVPA
     */
    void readTableNames(std::map<std::string,std::vector<std::string> >* TableNames);
    /**
     * @brief rescales the passed matrix by: log base 10
     * @param matrix to rescale
     */
    void rescaleMatrixLog(boost::numeric::ublas::matrix<double>& matrix);
    /**
     * @brief rescales the passed matrix by: (X - Xmin) / (Xmax - Xmin) where X is an element of the matrix
     * @param matrix : the matrix to rescale
     */
    void rescaleMatrixMinMax(boost::numeric::ublas::matrix<double>& matrix);

    void reset(boost::numeric::ublas::matrix<double>& mat,
               const double& resetValue);
    /**
     * @brief rounds a number to a specific number of decimal places
     * @param number : number to round
     * @param decimalPlaces : number of decimal places at which to perform the rounding
     * @return the rounded number
     */
    double round(double number,
                 int decimalPlaces);
    /**
     * @brief splits a string by a specific delimeter
     * @param main : the string to split
     * @param delim : the delimeter to split the string by
     * @param str1 : the substring to the left of the delimeter
     * @param str2 : the substring to the right of the delimeter
     */
    void split(std::string main,
               std::string delim,
               std::string& str1,
               std::string& str2);
    /**
     * @brief appends the new vector to the current vector
     * @param newVec : vector to be appended
     * @param mainVec : the resultant appended vector
     */
    void append(std::vector<double>& newVec,
                std::vector<double>& currentVec);

}

